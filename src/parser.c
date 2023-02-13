#include "parser.h"

#include <clang-c/Index.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

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

const char *parser_help(void) { return "<pattern> - index all files matching pattern"; }

CommandHandler(indexer_handler) {
    int result_code = EXIT_SUCCESS;

    glob_t glob_result;
    int return_value;

    return_value = glob("*.c", GLOB_TILDE, NULL, &glob_result);
    if (return_value != 0) {
        fprintf(stderr, "Error: %d\n", return_value);
        return 1;
    }

    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        char *file_name = glob_result.gl_pathv[i];
        fprintf(stdout, "%s\n", file_name);
        CXTranslationUnit tu = parse_file(file_name, index);
        if (!tu)
            result_code = EXIT_FAILURE;
        disposeTranslationUnit(tu);
    }

    globfree(&glob_result);

    return result_code;
}
