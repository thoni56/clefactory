#include "references.h"

#include <clang-c/Index.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

static enum CXVisitorResult visitReference(void *context, CXCursor cursor, CXSourceRange range) {
    CXFile file;
    unsigned line, column;
    CXSourceLocation location = clang_getExpansionLocation();
    clang_getSpellingLocation(location, NULL, &line, &column, NULL);
    printf("Reference found at %d:%d\n", line, column);
    return CXVisit_Continue;
}

int references_handler(const char *arguments[]) {
    CXIndex index = createIndex(0, 0);

    int result_code = EXIT_SUCCESS;

    CXTranslationUnit tu = parseTranslationUnit(index, arguments[0], NULL, 0, NULL, 0, CXTranslationUnit_None);
    if (!tu) {
        return 1;
    }

    CXFile file = clang_getFile(tu, arguments[0]);
    unsigned line = 10, column = 2;
    CXSourceLocation location = clang_getLocation(tu, file, line, column);

    CXCursor cursor = clang_getCursor(tu, location);
    CXCursor reference = clang_getCursorReferenced(cursor);
    if (!clang_Cursor_isNull(reference)) {
        cursor = reference;
    }

    CXCursorAndRangeVisitor visitor = {};
    visitor.context = &cursor;
    visitor.visit = visitReference;

    clang_findReferencesInFile(cursor, file, visitor);

    disposeIndex(index);

    return result_code;
}
