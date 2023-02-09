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

    CXTranslationUnit tu;
    expect(parseTranslationUnit, will_return(&tu));

    CXFile file = (CXFile)0xfefefefe;
    expect(getFile, when(unit, is_equal_to(&tu)),
           will_return(file));

    CXSourceLocation location;
    expect(getLocation, will_return(&location));

    CXCursor cursor;
    expect(getCursor, will_return(&cursor));

    CXSourceRange range;
    expect(getCursorExtent, will_return(&range));

    CXSourceLocation start;
    expect(getRangeStart, will_return(&start));

    expect(getExpansionLocation);

    CXString file_name;
    expect(getFileName, will_return(&file_name));
    expect(getCString, will_return("fileName.c"));

    CXSourceLocation end;
    expect(getRangeEnd, will_return(&end));

    expect(getExpansionLocation);
    expect(getFileName, will_return(&file_name));
    expect(getCString, will_return("fileName.c"));

    CXCursor referenced;
    expect(getCursorReferenced, will_return(&referenced));
    expect(cursor_isNull, will_return(true));

    expect(findReferencesInFile);

    expect(disposeTranslationUnit);

    CXIndex index = (CXIndex)0xfefefefe;
    references_handler(index, arguments);
}
