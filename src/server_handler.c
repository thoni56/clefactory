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

ResultCode handle_server_response(FILE *server_response_channel, FILE *client_response_channel) {
    char input[BUFFER_SIZE];
    ResultCode rc = RC_OK;

    if (readLine(input, sizeof(input), server_response_channel) != NULL) {
        readLine(input, sizeof(input), stdin);
        if (strcmp(input, "\r\n") != 0) {
            log_error("Missing message separator");
            rc = RC_MISSING_MESSAGE_SEPARATOR;
        }

        cJSON *root = jsonParse(input);
        cJSON *method = jsonGetObjectItem(root, "method");
        if (method != NULL) {
            log_trace("Client responded with '%s' response", method->valuestring);
            jsonSend(root, client_response_channel);
            jsonDelete(root);
        } else {
            log_warn("Received an invalid JSON-RPC message");
        }
    } else {
        log_fatal("Server closed pipe");
        rc = RC_SERVER_CLOSED_PIPE;
    }
    return rc;
}
