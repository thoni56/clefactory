#include "server_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"
#include "io.h"
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

ResultCode handle_server_response(int server_response_pipe, int client_response_pipe) {
    char input[BUFFER_SIZE];

    int nbytes = readPipe(server_response_pipe, input, BUFFER_SIZE);
    if (nbytes == -1) {
        return RC_RECEIVING_FROM_SERVER;
    } else if (nbytes == 0) {
        log_fatal("Server closed pipe");
    } else {
        log_trace("Received server response \"%s\"", input);
        if (writePipe(client_response_pipe, input, nbytes) == -1) {
            return RC_SENDING_TO_CLIENT;
        }
    }
    return RC_OK;
}
