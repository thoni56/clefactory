#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "references.h"

#include "clang_adapter.mock"


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

    CXCursor referenced;
    expect(getCursorReferenced, will_return(&referenced));
    expect(cursor_isNull, will_return(true));

    expect(findReferencesInFile);

    expect(disposeTranslationUnit);

    CXIndex index = (CXIndex)0xfefefefe;
    FileItem null_fileItem = {.fileName = NULL};
    FileTable fileTable = &null_fileItem;
    references_handler(index, fileTable, arguments);
}
