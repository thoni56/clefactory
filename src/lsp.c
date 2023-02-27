#include "lsp.h"

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
#include "client_handler.h"


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
            ResultCode rc = handle_client_request(server_request_pipe);
            if (rc != RC_OK)
                return rc;
        }
    }
}
