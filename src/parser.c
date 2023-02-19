#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "clang_adapter.h"

static CXTranslationUnit parse_file(const char *file_name, CXIndex index) {
    CXTranslationUnit tu =
        parseTranslationUnit(index, file_name, 0, 0, 0, 0, CXTranslationUnit_KeepGoing);
    if (!tu) {
        fprintf(stderr, "Error parsing translation unit '%s'\n", file_name);
    }
    return tu;
}

int parse_files(FileTable fileTable, CXIndex index) {
    for (FileItem *fileItem = &fileTable[0]; fileItem->fileName != NULL; fileItem++)
        fileItem->unit = parse_file(fileTable[0].fileName, index);
    return EXIT_SUCCESS;
}
