#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "exports.h"

#include "clang_adapter.mock"


Describe(Exports);
BeforeEach(Exports) {}
AfterEach(Exports) {}


Ensure(Exports, can_run_empty_test) {
}
