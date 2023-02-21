#include "lsp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.h"
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

#define BUFFER_SIZE 1000

// Communication with downstream server is over the pipes and with the
// upstream client over stdin/stdout
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
        sleep(1);
        fd_set tmp_inputs = inputs;
        if (select(max_fd + 1, &tmp_inputs, NULL, NULL, NULL) == -1) {
            log_fatal("Error during select()");
            return RC_SELECT_ERROR;
        }
        if (FD_ISSET(server_response_pipe, &tmp_inputs)) {
            char input[BUFFER_SIZE];

            int nbytes = read(server_response_pipe, input, BUFFER_SIZE);
            if (nbytes == -1) {
                perror("read");
                exit(1);
            } else if (nbytes == 0) {
                log_fatal("Server closed pipe");
                FD_CLR(server_response_pipe, &inputs);
            } else {
                log_trace("Received server response \"%s\"", input);
                if (write(client_response_pipe, input, nbytes) == -1) {
                    perror("write");
                    exit(1);
                }
            }
        } else if (FD_ISSET(client_request_pipe, &tmp_inputs)) {
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
                        write(server_request_pipe, input, strlen(input));
                        return EXIT_SUCCESS;
                    } else {
                        log_warn("Received an unknown request with method '%s'",
                                 method->valuestring);
                    }
                } else {
                    log_warn("Received an invalid JSON-RPC message");
                }
                jsonDelete(root);
                write(server_request_pipe, input, strlen(input));
            } else {
                log_error("Broken connection to client");
                return RC_BROKEN_INPUT_FROM_CLIENT;
            }
        }
    }
}
