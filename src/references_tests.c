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
    const char *arguments[] = {"filename.c", "12", "21"};

    expect(createIndex);
    CXTranslationUnit tu;
    expect(parseTranslationUnit, will_return(&tu));

    CXFile file = (CXFile)0xfefefefe;
    expect(getFile, when(unit, is_equal_to(&tu)),
           will_return(file));

    CXSourceLocation location;
    expect(getLocation, will_return(&location));

    CXCursor cursor;
    expect(getCursor, will_return(&cursor));

    expect(disposeTranslationUnit);
    expect(disposeIndex);

    references_handler(arguments);
}
