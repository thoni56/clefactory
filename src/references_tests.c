#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "references.h"

#include "clang_adaptor.mock"


Describe(References);
BeforeEach(References) {}
AfterEach(References) {}


Ensure(References, can_run_empty_test) {
}
