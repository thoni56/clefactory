#include "lsp.h"

#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.h"
#include "io.h"
#include "json_adapter.h"
#include "log.h"
#include "process.h"
#include "server_handler.h"

ResultCode lsp_inject(const char *program_name, int input_pipe[], int output_pipe[]) {
    pid_t pid;

    // Create input and output pipes
    if (pipe(input_pipe) < 0 || pipe(output_pipe) < 0) {
        return RC_PIPE_CREATION_ERROR;
    }

    // Fork a new process to handle the LSP server
    pid = fork_process();
    if (pid < 0) {
        return RC_FORK_FAILED;
    } else if (pid == 0) {
        // Child process
        close(input_pipe[1]);  // Close unused write end of input pipe
        close(output_pipe[0]); // Close unused read end of output pipe
        launch_server(input_pipe[0], output_pipe[1], program_name);
    } else {
        // Parent process
        close(input_pipe[0]);  // Close unused read end of input pipe
        close(output_pipe[1]); // Close unused write end of output pipe
    }
    return RC_OK;
}

static int max(int a, int b) { return a > b ? a : b; }

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
        if (fgets(input, sizeof(input), stdin) != NULL) {
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

// Communication with downstream server is over the pipes and with the
// upstream client over stdin/stdout, let's figure out who sends a message to us
// and dispatch it.
ResultCode lsp_repl(int server_request_pipe, int server_response_pipe, FileTable fileTable,
                    CXIndex index) {
    int client_request_pipe = STDIN_FILENO;
    int client_response_pipe = STDOUT_FILENO;

    fd_set inputs;
    int max_fd = max(client_request_pipe, server_response_pipe);

    FD_ZERO(&inputs);
    FD_SET(client_request_pipe, &inputs);
    FD_SET(server_response_pipe, &inputs);

    while (true) {
        fd_set tmp_inputs = inputs;
        if (select(max_fd + 1, &tmp_inputs, NULL, NULL, NULL) == -1) {
            log_fatal("Error during select()");
            return RC_SELECT_ERROR;
        }
        if (FD_ISSET(server_response_pipe, &tmp_inputs)) {
            ResultCode rc = handle_server_response(server_response_pipe, client_response_pipe);
            if (rc == RC_ERROR_RECEIVING_FROM_SERVER || rc == RC_SERVER_CLOSED_PIPE) {
                FD_CLR(server_response_pipe, &inputs);
                return rc;
            }
        } else if (FD_ISSET(client_request_pipe, &tmp_inputs)) {
            ResultCode rc = RC_OK;
            rc = parseRpcHeader();
            if (rc != RC_OK)
                return rc;
            char input[BUFFER_SIZE];
            if (fgets(input, sizeof(input), stdin) != NULL) {
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
                        return RC_OK;
                    } else {
                        log_warn("Received an unknown request with method '%s'",
                                 method->valuestring);
                    }
                } else {
                    log_warn("Received an invalid JSON-RPC message");
                }
                send_json_rpc_message(server_request_pipe, root);
                jsonDelete(root);
                fgets(input, sizeof(input), stdin);
                if (strcmp(input, "\r\n") != 0) {
                    log_error("Missing message separator");
                    return RC_MISSING_MESSAGE_SEPARATOR;
                }
            } else {
                log_error("Broken connection to client");
                return RC_BROKEN_INPUT_FROM_CLIENT;
            }
        }
    }
}
