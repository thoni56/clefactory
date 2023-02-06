#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "indexer.h"

#include "clang_adaptor.mock"


Describe(Indexer);
BeforeEach(Indexer) {}
AfterEach(Indexer) {}


Ensure(Indexer, can_run_empty_test) {
}
