#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

extern int main(int argc, char *argv[]);

#include "dispatcher.mock"


Describe(Main);
BeforeEach(Main) {}
AfterEach(Main) {}

Ensure(Main, will_exit_when_dispatcher_exits) {
    char *argv[] = {"crefactory"};

    expect(read_and_execute_commands);
    assert_that(main(1, argv), is_equal_to(EXIT_SUCCESS));
}
