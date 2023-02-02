#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

static CXFile current_file;

static enum CXChildVisitResult printCursor(CXCursor cursor, CXCursor parent,
                                           CXClientData client_data) {
    enum CXCursorKind kind = getCursorKind(cursor);
    if (kind == CXCursor_VarDecl || kind == CXCursor_FunctionDecl) {
        CXString name = getCursorSpelling(cursor);
        enum CX_StorageClass storage = cursor_getStorageClass(cursor);
        CXSourceLocation location = getCursorLocation(cursor);
        CXFile file;
        getSpellingLocation(location, &file, NULL, NULL, NULL);

        if (file_isEqual(file, current_file) && storage == CX_SC_None) {
            printf("%s\n", getCString(name));
        }
    }
    return CXChildVisit_Continue;
}

int exports_handler(const char *arguments[]) {
    CXIndex index = createIndex(0, 0);
    CXTranslationUnit tu =
        parseTranslationUnit(index, arguments[0], 0, 0, 0, 0, CXTranslationUnit_KeepGoing);
    if (!tu) {
        fprintf(stderr, "Error parsing translation unit\n");
        return 1;
    }

    CXCursor cursor = getTranslationUnitCursor(tu);

    current_file = getFile(tu, arguments[0]);

    visitChildren(cursor, printCursor, NULL);

    disposeTranslationUnit(tu);
    disposeIndex(index);

    return EXIT_SUCCESS;
}
