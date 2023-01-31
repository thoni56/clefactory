#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "dispatcher.h"


Describe(Dispatcher);
BeforeEach(Dispatcher) {}
AfterEach(Dispatcher) {}


static FILE *create_an_empty_file(const char *input) {
    char temp_file_template[] = "/tmp/temp_file.XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd == -1) {
        perror("Error creating temporary file");
        exit(EXIT_FAILURE);
    }
    FILE *output = fdopen(fd, "w");
    fprintf(output, "%s", input);
    return fdopen(fd, "r");
}


Ensure(Dispatcher, will_exit_immediately_on_empty_input) {
	FILE *channel = create_an_empty_file("");
    DispatchTable table[1];
    dispatch_commands(channel, table);
    assert_that(true);
}

bool handler_has_been_called = false;
static int aboutHandler(const char *command) {
    handler_has_been_called = true;
    return 0;
}

Ensure(Dispatcher, will_call_handler_for_matching_command) {
    FILE *channel = create_an_empty_file("about\n");
    DispatchTable table[] = {{"about", aboutHandler},
                           {"", NULL}};
    dispatch_commands(channel, table);
    assert_that(handler_has_been_called);
}
