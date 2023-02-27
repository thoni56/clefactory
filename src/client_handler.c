#include "client_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "io.h"
#include "json_adapter.h"
#include "log.h"


#define BUFFER_SIZE 10000

static ResultCode send_json_rpc_message(int pipe, cJSON *json) {
    char *payload = jsonPrint(json);

    // Create the message header
    int length = strlen(payload);
    char header[1000];
    snprintf(header, sizeof(header),
             "Content-Length: %d\r\nContent-type: application/vscode-jsonrpc;charset=utf-8\r\n\r\n", length);

    // Concatenate the header and message and delimiter
    char *delimiter = "\r\n\r\n";
    int message_length = strlen(header) + strlen(payload) + strlen(delimiter);
    char *buffer = malloc(message_length);
    strcpy(buffer, header);
    strcat(buffer, payload);
    strcat(buffer, delimiter);

    // Send the message
    int result = writePipe(pipe, buffer, message_length);
    fsync(pipe);
    free(buffer);
    return result;
}

static ResultCode parseRpcHeader(void) {
    int content_length = 0;
    ResultCode rc = RC_OK;
    for (;;) {
        char input[BUFFER_SIZE];
        if (readLine(input, sizeof(input), stdin) != NULL) {
            if (strcmp(input, "\r\n") == 0) { // End of header
                if (content_length == 0) {
                    log_error("Client sent incomplete header");
                    rc = RC_CLIENT_SENT_INCOMPLETE_HEADER;
                }
                break;
            }
            char *buffer_part = strtok(input, " ");
            if (strcmp(buffer_part, "Content-Length:") == 0) {
                buffer_part = strtok(NULL, "\n");
                content_length = atoi(buffer_part);
            }

        } else {
            log_error("Broken connection to client");
            rc = RC_BROKEN_INPUT_FROM_CLIENT;
        }
    }

    return rc;
}


ResultCode handle_client_request(int server_request_pipe) {
	ResultCode rc;
    rc = parseRpcHeader();
    char input[BUFFER_SIZE];
    if (readLine(input, sizeof(input), stdin) != NULL) {
        cJSON *root = jsonParse(input);
        cJSON *method = jsonGetObjectItem(root, "method");
        if (method != NULL) {
            if (strcmp(method->valuestring, "initialize") == 0) {
                log_trace("Received an 'initialize' request");
            } else if (strcmp(method->valuestring, "shutdown") == 0) {
                log_trace("Received a 'shutdown' request");
            } else if (strcmp(method->valuestring, "exit") == 0) {
                log_trace("Received an 'exit' request");
                send_json_rpc_message(server_request_pipe, root);
                rc = RC_OK;
            } else {
                log_warn("Received an unknown request with method '%s'",
                         method->valuestring);
            }
        } else {
            log_warn("Received an invalid JSON-RPC message");
        }
        send_json_rpc_message(server_request_pipe, root);
        jsonDelete(root);
        readLine(input, sizeof(input), stdin);
        if (strcmp(input, "\r\n") != 0) {
            log_error("Missing message separator");
            rc = RC_MISSING_MESSAGE_SEPARATOR;
        }
    } else {
        log_error("Broken connection to client");
        rc = RC_BROKEN_INPUT_FROM_CLIENT;
    }
    return rc;
}
