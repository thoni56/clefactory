#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#include "dispatcher.h"
#include "exports.h"
#include "filemanager.h"
#include "indexer.h"
#include "includes.h"
#include "references.h"

#define MAX_LINE_LENGTH 3000


static int about_handler(CXIndex index, const char *arguments[]) {
    printf("This is Clefactory v0.0\n");
    return EXIT_SUCCESS;
}

static const char *about_help(void) { return ""; }

static int help_handler(CXIndex index, const char *arguments[]);

static DispatchTable dispatchTable[] = {
    {"about",      about_handler,      about_help     },
    {"exports",    exports_handler,    exports_help   },
    {"includes",   includes_handler,   includes_help  },
    {"indexer",    indexer_handler,    indexer_help   },
    {"references", references_handler, references_help},
    {"?",          help_handler,       NULL           }, /* NOTE: Help will stop at "?" */
    {"help",       help_handler,       NULL           },
    {"",           NULL,               NULL           }
};

static int help_handler(CXIndex index, const char *arguments[]) {
    for (DispatchTable *t = dispatchTable; strcmp(t->command, "?") != 0; t++)
        fprintf(stdout, "%s %s\n", t->command, t->help());
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    char command[MAX_LINE_LENGTH];

    // TODO: Set CWD to argv[1] if available

    // TODO: create a compilation database object using clang_CompilationDatabase_fromDirectory()
    // TODO: use that to get compile flags and options for a specific translation unit
    FileTable fileTable = getFilesFromCurrentDirectory();
    UNUSED fileTable;

    CXIndex index = createIndex(0, 0);

    // TODO: create a table of filenames of all translation units,
    // all their dependent included files and their latest
    // modification time ...

    fprintf(stdout, "> ");
    while (fgets(command, sizeof(command), stdin) != NULL) {
        // TODO: ... so that we can refresh the index if needed,
        // before doing the operation
        int error_code = dispatch_command(index, command, dispatchTable);
        if (error_code != 0) {
            fprintf(stderr, "Error: %d\n", error_code);
        }
        fprintf(stdout, "> ");
    }
    return EXIT_SUCCESS;
}
