#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "references.h"

#include "clang_adaptor.mock"


Describe(References);
BeforeEach(References) {}
AfterEach(References) {}


Ensure(References, can_find_references) {
    const char *arguments[] = {"filename.c", NULL};

    expect(createIndex);

    expect(disposeIndex);

    references_handler(arguments);
}
