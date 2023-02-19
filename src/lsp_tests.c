#include "filemanager.h"
#include "lsp.h"

#include <cgreen/assertions.h>
#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/internal/unit_implementation.h>
#include <cgreen/unit.h>
#include <unistd.h>

#include "server_handler.mock"
#include "process.mock"


Describe(Lsp);
BeforeEach(Lsp) {}
AfterEach(Lsp) {}

#define BUFFER_SIZE 1000

Ensure(Lsp, will_close_when_clients_input_pipe_closes) {
    int input_pipe[2], output_pipe[2];
    char buffer[BUFFER_SIZE];
    int nbytes;

    if (pipe(input_pipe) == -1 || pipe(output_pipe) == -1) {
        perror("pipe");
        exit(1);
    }

    // Send data to input pipe
    if (write(input_pipe[1], "hello, world!", 13) == -1) {
        perror("write");
        fail_test("write");
    }

    // Child process: call handler and exit
    FileItem fileItem = {.fileName = NULL};
    FileTable fileTable = &fileItem;
    CXIndex index = (CXIndex)0xfbfbfb;
    lsp_listener(fileTable, index, input_pipe[0], output_pipe[1]);

    // Read manipulated data from output pipe
    nbytes = read(output_pipe[0], buffer, BUFFER_SIZE);
    if (nbytes == -1) {
        perror("read");
        fail_test("read");
    }
    buffer[nbytes-1] = '\0';
    assert_that(buffer, is_equal_to_string("hello, world!"));

    close(input_pipe[1]);
    close(output_pipe[0]);
    close(input_pipe[0]);
    close(output_pipe[1]);

    pass_test();
}

Ensure(Lsp, will_launch_program_on_inject) {
    int input_pipe[2], output_pipe[2];

    expect(fork_process);
    expect(launch_server); //, when(program, is_equal_to_string("the_program")));

    lsp_inject("the_program", input_pipe, output_pipe);
}
