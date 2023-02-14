#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clang_adaptor.h"
#include "common.h"
#include "filemanager.h"
#include "repl.h"
#include "lsp.h"


typedef enum { NO_MODE, CLI_MODE, LSP_MODE } Mode;

static Mode decode_arguments(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--cli") == 0)
            return CLI_MODE;
        else
            return LSP_MODE;
    } else {
        fprintf(stderr, "Need to select '--cli' or '--lsp'\n");
        return NO_MODE;
    }
}

int main(int argc, char *argv[]) {
    // TODO: options handling... For now:
    Mode mode = decode_arguments(argc, argv);
    if (mode == NO_MODE)
        return EXIT_FAILURE;

    // TODO: Set CWD to argv[1] if available

    // TODO: create a compilation database object using clang_CompilationDatabase_fromDirectory()
    // TODO: use that to get compile flags and options for a specific translation unit
    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    CXIndex index = createIndex(0, 0);

    // TODO: create a table of filenames of all translation units,
    // all their dependent included files and their latest
    // modification time ...
    if (mode == CLI_MODE)
        cli_repl(fileTable, index);
    else {
        lsp_init();
        lsp_event_loop(fileTable, index);
    }

    disposeIndex(index);
    freeFileTable(fileTable);

    return EXIT_SUCCESS;
}
