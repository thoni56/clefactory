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
            break;
        }
    }
    return rc;
}


ResultCode handle_client_request(FILE *server_request_channel, FILE *client_request_pipe) {
    ResultCode rc;
    rc = parseRpcHeader();
    if (rc != RC_OK)
        return rc;

    char input[BUFFER_SIZE];
    if (readLine(input, sizeof(input), stdin) != NULL) {
        readLine(input, sizeof(input), stdin);
        if (strcmp(input, "\r\n") != 0) {
            log_error("Missing message separator");
            rc = RC_MISSING_MESSAGE_SEPARATOR;
        }

        cJSON *root = jsonParse(input);
        cJSON *method = jsonGetObjectItem(root, "method");
        if (method != NULL) {
            log_trace("Client responded with '%s' response", method->valuestring);
            jsonSend(root, server_request_channel);
            jsonDelete(root);
        } else {
            log_warn("Received an invalid JSON-RPC message");
        }
        jsonSend(root, server_request_channel);
        jsonDelete(root);
    } else {
        log_error("Broken connection to client");
        rc = RC_BROKEN_INPUT_FROM_CLIENT;
    }
    return rc;
}
