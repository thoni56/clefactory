#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include "io.mock"

Describe(ServerHandler);
BeforeEach(ServerHandler) {}
AfterEach(ServerHandler) {}

Ensure(ServerHandler, can_run_empty_test) {
}
