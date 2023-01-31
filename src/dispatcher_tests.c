#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "dispatcher.h"


Describe(Dispatcher);
BeforeEach(Dispatcher) {}
AfterEach(Dispatcher) {}


static FILE *create_an_empty_file() {
    char temp_file_template[] = "/tmp/temp_file.XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd == -1) {
        perror("Error creating temporary file");
        exit(EXIT_FAILURE);
    }
    return fdopen(fd, "r");
}


Ensure(Dispatcher, will_exit_immediately_on_empty_input) {
	FILE *channel = create_an_empty_file();
    dispatch_commands(channel);
    assert_that(true);
}
