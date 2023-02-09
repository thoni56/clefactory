#include "references.h"

#include <clang-c/Index.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

static enum CXVisitorResult visitReference(void *context, CXCursor cursor, CXSourceRange range) {
    CXSourceLocation sourceLocation = getRangeStart(range);
    CXFile file;
    unsigned int line;
    unsigned int column;
    getExpansionLocation(sourceLocation, &file, &line, &column, NULL);

    CXString fileName = getFileName(file);
    printf("Reference at %s:%d,%d\n", getCString(fileName), line, column);
    disposeString(fileName);
    return CXVisit_Continue;
}

const char *references_help(void) {
    return "<filename> <line> <column> - prints all locations where the symbol at that location is referenced";
}

int references_handler(const char *arguments[]) {
    CXIndex index = createIndex(0, 0);

    int result_code = EXIT_SUCCESS;

    CXTranslationUnit tu = parseTranslationUnit(index, arguments[0], NULL, 0, NULL, 0, CXTranslationUnit_None);
    if (!tu) {
        return 1;
    }

    CXFile file = getFile(tu, arguments[0]);
    unsigned line = atoi(arguments[1]);
    unsigned column = atoi(arguments[2]);
    CXSourceLocation location = getLocation(tu, file, line, column);

    CXCursor cursor = getCursor(tu, location);

    CXSourceRange range = clang_getCursorExtent(cursor);

    CXSourceLocation start = clang_getRangeStart(range);
    CXFile cursorFile;
    unsigned int startLine;
    unsigned int startColumn;
    clang_getExpansionLocation(start, &cursorFile, &startLine, &startColumn, NULL);
    CXString fileName = getFileName(file);
    printf("Cursor starts at %s %d:%d\n", getCString(fileName), startLine, startColumn);

    CXSourceLocation end = clang_getRangeEnd(range);
    clang_getExpansionLocation(end, &cursorFile, &startLine, &startColumn, NULL);
    fileName = getFileName(file);
    printf("Cursor ends at %s %d:%d\n", getCString(fileName), startLine, startColumn);

    CXCursor reference = getCursorReferenced(cursor);
    if (!cursor_isNull(reference)) {
        cursor = reference;
    }

    CXCursorAndRangeVisitor visitor = {NULL, visitReference};

    findReferencesInFile(cursor, file, visitor);

    disposeIndex(index);

    return result_code;
}
