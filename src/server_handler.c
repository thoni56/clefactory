#include "server_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void launch_server(int input_pipe[], int output_pipe[], const char program[]) {
    close(input_pipe[1]);  // Close unused write end of input pipe
    close(output_pipe[0]); // Close unused read end of output pipe

    // Duplicate input and output pipe read and write ends to stdin and stdout
    dup2(input_pipe[0], STDIN_FILENO);
    dup2(output_pipe[1], STDOUT_FILENO);

    // Launch the LSP server using exec
    execlp(program, program, NULL);

    // If we get here the exec() failed.
    perror("Error launching LSP server");
    exit(1);
}
