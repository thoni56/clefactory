#include "lsp.h"
#include "error.h"
#include "json_adapter.h"
#include "process.h"
#include "server_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "log.h"

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

ResultCode lsp_init(int server_request_pipe, int server_response_pipe) {
    char buffer[1000];

    // Write data to the LSP server
    if (write(server_request_pipe,
              "{\"jsonrpc\":\"2.0\",\"method\":\"initialize\",\"params\":{}}\r\n", 54) < 0) {
        return RC_WRITING_TO_SERVER;
    }

    // Read data from the LSP server
    if (read(server_response_pipe, buffer, 1000) < 0) {
        return RC_READING_FROM_SERVER;
    }

    log_trace("Response from LSP server: %s", buffer);
    return RC_OK;
}

ResultCode lsp_repl(int server_request_pipe, int server_response_pipe, FileTable fileTable,
                    CXIndex index) {
    char input[1000];
    while (fgets(input, sizeof(input), stdin)) {
        cJSON *root = jsonParse(input);
        cJSON *method = jsonGetObjectItem(root, "method");
        if (method != NULL) {
            if (strcmp(method->valuestring, "initialize") == 0) {
                log_trace("Received an 'initialize' message");
            } else if (strcmp(method->valuestring, "shutdown") == 0) {
                log_trace("Received a 'shutdown' message");
            } else if (strcmp(method->valuestring, "exit") == 0) {
                log_trace("Received an 'exit' message");
                write(server_request_pipe, input, strlen(input));
                return EXIT_SUCCESS;
            } else {
                log_warn("Received an unknown message with method '%s'", method->valuestring);
            }
        } else {
            log_warn("Received an invalid JSON-RPC message");
        }
        jsonDelete(root);
        write(server_request_pipe, input, strlen(input));
    }
    log_error("Broken connection to client");
    return RC_BROKEN_INPUT_FROM_CLIENT;
}

int lsp_listener(FileTable fileTable, CXIndex index, int client_input_pipe,
                 int client_output_pipe) {
    char buffer[1000];
    int nbytes = read(client_input_pipe, buffer, 1000);
    if (nbytes == -1) {
        perror("client read");
    }
    write(client_output_pipe, buffer, strlen(buffer));
    return EXIT_SUCCESS;
}
