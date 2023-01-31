#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "dispatcher.h"

Describe(Dispatcher);
BeforeEach(Dispatcher) {}
AfterEach(Dispatcher) {}

static const char **about_arguments;
static bool about_handler_has_been_called = false;
static int aboutHandler(const char *arguments[]) {
    about_handler_has_been_called = true;
    about_arguments = arguments;
    return 0;
}

static bool help_handler_has_been_called = false;
static int helpHandler(const char *arguments[]) {
    help_handler_has_been_called = true;
    return 0;
}

static DispatchTable dispatchTable[] = {
    {"about", aboutHandler},
    {"help",  helpHandler },
    {"",      NULL        }
};

Ensure(Dispatcher, will_call_handler_for_matching_command) {
    dispatch_command("about\n", dispatchTable);
    assert_that(about_handler_has_been_called);
    dispatch_command("help\n", dispatchTable);
    assert_that(help_handler_has_been_called);
}

Ensure(Dispatcher, will_send_array_terminated_by_NULL_pointer_to_handler) {}

Ensure(Dispatcher, will_give_rest_of_command_to_handler) {
    dispatch_command("about something\n", dispatchTable);
    assert_that(about_arguments[0], is_equal_to_string("something"));
    assert_that(about_arguments[1], is_null);
}
