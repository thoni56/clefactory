#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <cgreen/unit.h>

#include "includes.h"

#include "clang_adapter.mock"


Describe(IncludesHandler);
BeforeEach(IncludesHandler) {}
AfterEach(IncludesHandler) {}

Ensure(IncludesHandler, can_run_empty_test) {}
