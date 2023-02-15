#include "lsp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void lsp_init(void) {}

int lsp_event_loop(FileTable fileTable, CXIndex index) { return EXIT_SUCCESS; }

int lsp_listener(FileTable fileTable, CXIndex index, int client_input_pipe, int client_output_pipe) {
    char buffer[1000];
    int nbytes = read(client_input_pipe, buffer, 1000);
    if (nbytes == -1) {
        perror("client read");
    }
    write(client_output_pipe, buffer, strlen(buffer));
    return EXIT_SUCCESS;
}
