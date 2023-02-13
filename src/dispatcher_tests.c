#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "dispatcher.h"

Describe(Dispatcher);
BeforeEach(Dispatcher) {}
AfterEach(Dispatcher) {}

static const char *about_arguments[10];
static bool about_handler_has_been_called = false;
static CommandHandler(about_handler) {
    about_handler_has_been_called = true;
    for (int i = 0; arguments[i] != NULL; i++)
        about_arguments[i] = strdup(arguments[i]);
    return 0;
}

static bool help_handler_has_been_called = false;
static CommandHandler(help_handler) {
    help_handler_has_been_called = true;
    return 0;
}

static bool exit_handler_has_been_called = false;
static CommandHandler(exit_handler) {
    exit_handler_has_been_called = true;
    return 0;
}

static DispatchTable dispatchTable[] = {
    {"about", about_handler},
    {"help",  help_handler},
    {"exit",  exit_handler},
    {"",      NULL        }
};

static FileItem nullFileItem = { .fileName = NULL };
static FileTable emptyFileTable = &nullFileItem;

Ensure(Dispatcher, will_call_handler_for_matching_command) {
    dispatch_command(index, emptyFileTable, "about\n", dispatchTable);
    assert_that(about_handler_has_been_called);
    dispatch_command(index, emptyFileTable, "help\n", dispatchTable);
    assert_that(help_handler_has_been_called);
    dispatch_command(index, emptyFileTable, "exit\n", dispatchTable);
    assert_that(exit_handler_has_been_called);
}

Ensure(Dispatcher, will_return_EXIT_ERROR_when_no_handler_found) {
    assert_that(dispatch_command(index, emptyFileTable, "void", dispatchTable), is_equal_to(EXIT_FAILURE));
}

Ensure(Dispatcher, will_give_rest_of_command_to_handler) {
    dispatch_command(index, emptyFileTable, "about something or other\n", dispatchTable);
    assert_that(about_arguments[0], is_equal_to_string("something"));
    assert_that(about_arguments[1], is_equal_to_string("or"));
    assert_that(about_arguments[2], is_equal_to_string("other"));
    assert_that(about_arguments[3], is_null);
}
