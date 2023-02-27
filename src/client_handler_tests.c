#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include "client_handler.h"
#include "log.h"

#include "io.mock"
#include "json_adapter.mock"


Describe(ClientHandler);
BeforeEach(ClientHandler) {
    log_set_level(LOG_FATAL);
}
AfterEach(ClientHandler) {}


Ensure(ClientHandler, can_compile_with_an_empty_test) {
}
