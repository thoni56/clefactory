#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>

#include "repl.h"

#include "clang_adaptor.mock"
#include "dispatcher.mock"
#include "exports.mock"
#include "filemanager.mock"
#include "includes.mock"
#include "parser.mock"
#include "references.mock"
#include "units.mock"


Describe(Repl);
BeforeEach(Repl) {}
AfterEach(Repl) {}


Ensure(Repl, can_run_empty_test) {
}
