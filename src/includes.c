#include "includes.h"

#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adapter.h"
#include "filemanager.h"


static void inclusionVisitor(CXFile includedFile,
                             CXSourceLocation *inclusionStack,
                             unsigned includeLen,
                             CXClientData clientData)
{
    CXString fileName = getFileName(includedFile);
    printf("Included file: %s\n", getCString(fileName));
    disposeString(fileName);
}

const char *includes_help(void) {
    return "<filename> - prints all files included, recursively";
}

CommandHandler(includes_handler) {
    CXTranslationUnit tu =
        parseTranslationUnit(index, arguments[0], NULL, 0, NULL, 0, CXTranslationUnit_KeepGoing);
    if (!tu) {
        fprintf(stderr, "Error parsing translation unit\n");
        return 1;
    }

    getInclusions(tu, inclusionVisitor, NULL);
    disposeTranslationUnit(tu);

    return EXIT_SUCCESS;
}
