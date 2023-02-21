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
#include "json_adapter.mock"


Describe(Lsp);
BeforeEach(Lsp) {}
AfterEach(Lsp) {}


Ensure(Lsp, will_launch_program_on_inject) {
    int input_pipe[2], output_pipe[2];

    expect(fork_process);
    expect(launch_server); //, when(program, is_equal_to_string("the_program")));

    lsp_inject("the_program", input_pipe, output_pipe);
}
