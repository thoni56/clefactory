#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "dispatcher.h"


Describe(Dispatcher);
BeforeEach(Dispatcher) {}
AfterEach(Dispatcher) {}


bool about_handler_has_been_called = false;
static int aboutHandler(const char *command) {
    about_handler_has_been_called = true;
    return 0;
}

bool help_handler_has_been_called = false;
static int helpHandler(const char *command) {
    help_handler_has_been_called = true;
    return 0;
}

Ensure(Dispatcher, will_call_handler_for_matching_command) {
    DispatchTable table[] = {
        {"about", aboutHandler},
        {"help", helpHandler},
        {"", NULL}
    };
    dispatch_command("about\n", table);
    assert_that(about_handler_has_been_called);
    dispatch_command("help\n", table);
    assert_that(help_handler_has_been_called);
}
