#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

extern int main(int argc, char *argv[]);

#include "clang_adaptor.mock"
#include "filemanager.mock"
#include "repl.mock"


Describe(Main);
BeforeEach(Main) {}
AfterEach(Main) {}

Ensure(Main, can_run_an_empty_test) {
}
