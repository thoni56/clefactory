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
#include "options.h"


protected int main_(int argc, char *argv[]) {
    log_set_level(LOG_ERROR);
    ResultCode rc = decode_options(argc, argv);
    if (rc != RC_OK)
        return EXIT_FAILURE;

    if (options.logfile_path != NULL) {
        FILE *log_file = fopen(options.logfile_path, "w");
        log_add_fp(log_file, LOG_TRACE);
    }

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
