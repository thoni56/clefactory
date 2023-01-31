#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "dispatcher.h"


Describe(Dispatcher);
BeforeEach(Dispatcher) {}
AfterEach(Dispatcher) {}


bool handler_has_been_called = false;
static int aboutHandler(const char *command) {
    handler_has_been_called = true;
    return 0;
}

Ensure(Dispatcher, will_call_handler_for_matching_command) {
    DispatchTable table[] = {{"about", aboutHandler},
                             {"", NULL}};
    dispatch_command("about\n", table);
    assert_that(handler_has_been_called);
}
