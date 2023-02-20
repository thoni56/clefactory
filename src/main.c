#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clang_adapter.h"
#include "common.h"
#include "error.h"
#include "filemanager.h"
#include "log.h"
#include "repl.h"
#include "lsp.h"


typedef enum { NO_MODE, CLI_MODE, LSP_MODE } Mode;

static struct {
    Mode mode;
    char *lsp_server_path;
} options;

static ResultCode decode_options(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--cli") == 0) {
            options.mode = CLI_MODE;
            return RC_OK;
        } else if (strncmp("--lsp=", argv[1], 6) == 0) {
            options.lsp_server_path = argv[1] + 6;
            options.mode = LSP_MODE;
            return RC_OK;
        } else {
            fprintf(stderr, "Error in options\n");
            return RC_ERROR_IN_OPTIONS;
        }
    } else {
        fprintf(stderr, "Need to select '--cli' or '--lsp'\n");
        return RC_NO_MODE_SELECTED;
    }
}

protected int main_(int argc, char *argv[]) {
    log_set_level(LOG_ERROR);
    // TODO: options handling... For now:
    ResultCode rc = decode_options(argc, argv);
    if (rc != RC_OK)
        return EXIT_FAILURE;

    // TODO: Set CWD to argv[1] if available

    // TODO: create a compilation database object using clang_CompilationDatabase_fromDirectory()
    // TODO: use that to get compile flags and options for a specific translation unit
    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    CXIndex index = createIndex(0, 0);

    // TODO: create a table of filenames of all translation units,
    // all their dependent included files and their latest
    // modification time ...
    if (options.mode == CLI_MODE)
        cli_repl(fileTable, index);
    else {
        int pipe1[2], pipe2[2];
        lsp_inject(options.lsp_server_path, pipe1, pipe2);
        lsp_repl(pipe1[1], pipe2[0], fileTable, index);
    }

    disposeIndex(index);
    freeFileTable(fileTable);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    return main_(argc, argv);
}
