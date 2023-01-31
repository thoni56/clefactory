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
static int about_handler(const char *arguments[]) {
    about_handler_has_been_called = true;
    about_arguments = arguments;
    return 0;
}

static bool help_handler_has_been_called = false;
static int help_handler(const char *arguments[]) {
    help_handler_has_been_called = true;
    return 0;
}

static bool exit_handler_has_been_called = false;
static int exit_handler(const char *arguments[]) {
    exit_handler_has_been_called = true;
    return 0;
}

static DispatchTable dispatchTable[] = {
    {"about", about_handler},
    {"help",  help_handler},
    {"exit",  exit_handler},
    {"",      NULL        }
};

Ensure(Dispatcher, will_call_handler_for_matching_command) {
    dispatch_command("about\n", dispatchTable);
    assert_that(about_handler_has_been_called);
    dispatch_command("help\n", dispatchTable);
    assert_that(help_handler_has_been_called);
    dispatch_command("exit\n", dispatchTable);
    assert_that(exit_handler_has_been_called);
}

Ensure(Dispatcher, will_return_EXIT_ERROR_when_no_handler_found) {
    assert_that(dispatch_command("void", dispatchTable), is_equal_to(EXIT_FAILURE));
}

Ensure(Dispatcher, will_give_rest_of_command_to_handler) {
    dispatch_command("about something\n", dispatchTable);
    assert_that(about_arguments[0], is_equal_to_string("something"));
    assert_that(about_arguments[1], is_null);
}
