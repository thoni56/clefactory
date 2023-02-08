#include "references.h"

#include <clang-c/Index.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

static enum CXVisitorResult visitReference(void *context, CXCursor cursor, CXSourceRange range) {
    CXIdxLoc *location = (CXIdxLoc*)context;
    CXSourceLocation sourceLocation = indexLoc_getCXSourceLocation(*location);
    CXFile file;
    unsigned int line;
    unsigned int column;
    getExpansionLocation(sourceLocation, &file, &line, &column, NULL);

    CXString fileName = getFileName(file);
    printf("Reference at %s:%d\n", getCString(fileName), line);
    disposeString(fileName);
    return CXVisit_Continue;
}

int references_handler(const char *arguments[]) {
    CXIndex index = createIndex(0, 0);

    int result_code = EXIT_SUCCESS;

    CXTranslationUnit tu = parseTranslationUnit(index, arguments[0], NULL, 0, NULL, 0, CXTranslationUnit_None);
    if (!tu) {
        return 1;
    }

    CXFile file = getFile(tu, arguments[0]);
    unsigned line = 10, column = 2;
    CXSourceLocation location = getLocation(tu, file, line, column);

    CXCursor cursor = getCursor(tu, location);
    CXCursor reference = getCursorReferenced(cursor);
    if (!cursor_isNull(reference)) {
        cursor = reference;
    }

    CXCursorAndRangeVisitor visitor = {NULL, visitReference};

    findReferencesInFile(cursor, file, visitor);

    disposeIndex(index);

    return result_code;
}
