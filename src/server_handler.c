#include "server_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "error.h"
#include "io.h"
#include "json_adapter.h"
#include "log.h"

#define BUFFER_SIZE 10000

void launch_server(int input_pipe, int output_pipe, const char program[]) {
    // Duplicate input and output pipe read and write ends to stdin and stdout
    dup2(input_pipe, STDIN_FILENO);
    dup2(output_pipe, STDOUT_FILENO);

    // Launch the LSP server using exec
    execlp(program, program, NULL);

    // If we get here the exec() failed.
    log_fatal("Error launching LSP server");
    exit(EXIT_FAILURE);
}

static ResultCode parseRpcHeader(FILE *server_response_channel, int *length) {
    int content_length = 0;
    ResultCode rc = RC_OK;
    for (;;) {
        char input[BUFFER_SIZE];
        if (readLine(input, sizeof(input), server_response_channel) != NULL) {
            log_debug("server: -> '%s'", input);
            if (strcmp(input, "\r\n") == 0) { // End of header
                if (content_length == 0) {
                    log_error("Server sent incomplete header");
                    rc = RC_SERVER_SENT_INCOMPLETE_HEADER;
                }
                break;
            }
            char *buffer_part = strtok(input, " ");
            if (strcmp(buffer_part, "Content-Length:") == 0) {
                buffer_part = strtok(NULL, "\n");
                content_length = atoi(buffer_part);
            }

        } else {
            log_error("Server disconnected");
            rc = RC_BROKEN_INPUT_CHANNEL_FROM_SERVER;
            break;
        }
    }
    *length = content_length;
    return rc;
}

ResultCode handle_server_response(FILE *server_response_channel, FILE *client_response_channel) {
    char input[BUFFER_SIZE];
    ResultCode rc = RC_OK;
    int length = 0;

    rc = parseRpcHeader(server_response_channel, &length);
    if (rc != RC_OK)
        return rc;

    // We are relying on the Content-Length to contain everything up
    // to next package, including delimiter, as per ADR-0003
    if (readFile(server_response_channel, input, length) == length) {

        cJSON *root = jsonParse(input);
        cJSON *id = jsonGetObjectItem(root, "id");
        cJSON *result = jsonGetObjectItem(root, "result");
        if (result != NULL) {
            log_trace("server <- result (%d)", id->valueint);
            int result = jsonSend(root, client_response_channel);
            if (result == EOF)
                rc = RC_ERROR_SENDING_TO_CLIENT;
            jsonDelete(root);
        } else {
            cJSON *error = jsonGetObjectItem(root, "error");
            if (error != NULL) {
                log_trace("server <- error (%d)", id->valueint);
                int result = jsonSend(root, client_response_channel);
                if (result == EOF)
                    rc = RC_ERROR_SENDING_TO_CLIENT;
                jsonDelete(root);
            }
            log_warn("Server responded with an invalid JSON-RPC message");
        }

    } else {
        log_error("Server disconnected");
        rc = RC_BROKEN_INPUT_CHANNEL_FROM_SERVER;
    }
    return rc;
}
