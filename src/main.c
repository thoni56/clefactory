#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clang_adapter.h"
#include "common.h"
#include "filemanager.h"
#include "log.h"
#include "repl.h"
#include "lsp.h"


typedef enum { NO_MODE, CLI_MODE, LSP_MODE } Mode;

static char *lsp_server_bin;

static Mode decode_arguments(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--cli") == 0)
            return CLI_MODE;
        else if (strncmp("--lsp=", argv[1], 6) == 0) {
            lsp_server_bin = argv[1]+6;
            return LSP_MODE;
        }
        fprintf(stderr, "Error in options\n");
        return NO_MODE;
    } else {
        fprintf(stderr, "Need to select '--cli' or '--lsp'\n");
        return NO_MODE;
    }
}

protected int main_(int argc, char *argv[]) {
    log_set_level(LOG_ERROR);
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
        int pipe1[2], pipe2[2];
        lsp_inject(lsp_server_bin, pipe1, pipe2);
        lsp_init(pipe1[1], pipe2[0]);
        lsp_repl(pipe1[1], pipe2[0], fileTable, index);
    }

    disposeIndex(index);
    freeFileTable(fileTable);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    return main_(argc, argv);
}
