#include "indexer.h"

#include <clang-c/Index.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

static int index_file(const char *file_name, CXIndex index) {
    int result_code = EXIT_SUCCESS;

    CXTranslationUnit tu =
        parseTranslationUnit(index, file_name, 0, 0, 0, 0, CXTranslationUnit_KeepGoing);
    if (!tu) {
        fprintf(stderr, "Error parsing translation unit '%s'\n", file_name);
        result_code = EXIT_FAILURE;
    }

    disposeTranslationUnit(tu);

    return result_code;
}

const char *indexer_help(void) { return "<pattern> - index all files matching pattern"; }

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
        result_code = index_file(file_name, index);
    }

    globfree(&glob_result);

    disposeIndex(index);

    return result_code;
}
