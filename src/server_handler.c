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

static ResultCode parseRpcHeader(FILE *server_response_channel) {
    int content_length = 0;
    ResultCode rc = RC_OK;
    for (;;) {
        char input[BUFFER_SIZE];
        if (readLine(input, sizeof(input), server_response_channel) != NULL) {
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
    return rc;
}

ResultCode handle_server_response(FILE *server_response_channel, FILE *client_response_channel) {
    char input[BUFFER_SIZE];
    ResultCode rc = RC_OK;
    rc = parseRpcHeader(server_response_channel);
    if (rc != RC_OK)
        return rc;

    if (readLine(input, sizeof(input), server_response_channel) != NULL) {

        cJSON *root = jsonParse(input);
        cJSON *result = jsonGetObjectItem(root, "result");
        if (result != NULL) {
            log_trace("Server responded with result");
            int result = jsonSend(root, client_response_channel);
            if (result == EOF)
                rc = RC_ERROR_SENDING_TO_CLIENT;
            jsonDelete(root);
        } else {
            cJSON *error = jsonGetObjectItem(root, "error");
            if (error != NULL) {
                log_trace("Server responded with error");
                int result = jsonSend(root, client_response_channel);
                if (result == EOF)
                    rc = RC_ERROR_SENDING_TO_CLIENT;
                jsonDelete(root);
            }
            log_warn("Server responded with an invalid JSON-RPC message");
        }

        // Delimiter
        readLine(input, sizeof(input), server_response_channel);
        if (strcmp(input, "\r\n") != 0) {
            log_error("Missing message separator from server");
            rc = RC_MISSING_MESSAGE_DELIMITER_FROM_CLIENT;
        }

    } else {
        log_error("Server disconnected");
        rc = RC_BROKEN_INPUT_CHANNEL_FROM_SERVER;
    }
    return rc;
}
