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
        readLine(input, sizeof(input), server_response_channel);
        if (strcmp(input, "\r\n") != 0) {
            log_error("Missing message separator");
            rc = RC_MISSING_MESSAGE_SEPARATOR;
        }

        cJSON *root = jsonParse(input);
        cJSON *method = jsonGetObjectItem(root, "method");
        if (method != NULL) {
            log_trace("Server responded with '%s'", method->valuestring);
            int result = jsonSend(root, client_response_channel);
            if (result == EOF)
                rc = RC_ERROR_SENDING_TO_CLIENT;
            jsonDelete(root);
        } else {
            log_warn("Client responded with an invalid JSON-RPC message");
        }
    } else {
        log_error("Broken input channel from server");
        rc = RC_BROKEN_INPUT_CHANNEL_FROM_SERVER;
    }
    return rc;
}
