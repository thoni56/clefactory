#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "units.h"

#include "clang_adaptor.mock"


Describe(Units);
BeforeEach(Units) {}
AfterEach(Units) {}


Ensure(Units, can_run_empty_test) {
}
