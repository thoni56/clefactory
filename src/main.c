#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clang_adaptor.h"
#include "common.h"
#include "filemanager.h"
#include "repl.h"


int main(int argc, char *argv[]) {
    // TODO: Set CWD to argv[1] if available

    // TODO: create a compilation database object using clang_CompilationDatabase_fromDirectory()
    // TODO: use that to get compile flags and options for a specific translation unit
    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    CXIndex index = createIndex(0, 0);

    // TODO: create a table of filenames of all translation units,
    // all their dependent included files and their latest
    // modification time ...

    // TODO: options handling... For now:
    if (strcmp(argv[1], "--cli") == 0)
        cli_repl(fileTable, index);
    //else if (strcmp(argv[1], "--lsp") == 0)
    //  lsp_event_loop(fileTable, index);
    else
        fprintf(stderr, "Need to select '--cli' or '--lsp'\n");

    disposeIndex(index);
    freeFileTable(fileTable);

    return EXIT_SUCCESS;
}
