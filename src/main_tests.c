#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

extern int main(int argc, char *argv[]);

#include "clang_adaptor.mock"
#include "dispatcher.mock"
#include "exports.mock"
#include "filemanager.mock"
#include "includes.mock"
#include "parser.mock"
#include "references.mock"
#include "units.mock"


Describe(Main);
BeforeEach(Main) {}
AfterEach(Main) {}

Ensure(Main, can_run_an_empty_test) {
}
