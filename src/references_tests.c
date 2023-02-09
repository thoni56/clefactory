#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "references.h"

#include "clang_adaptor.mock"
#include "clang-c/Index.h"


Describe(References);
BeforeEach(References) {}
AfterEach(References) {}


Ensure(References, can_find_references) {
    const char *arguments[] = {"filename.c", NULL};

    expect(createIndex);
    CXTranslationUnit tu;
    expect(parseTranslationUnit, will_return(&tu));

    CXFile file;
    expect(getFile, when(unit, is_equal_to(&tu)),
           will_return(file));

    expect(disposeTranslationUnit);
    expect(disposeIndex);

    references_handler(arguments);
}
