#include "repl.h"

#include <stdlib.h>
#include <string.h>

#include "dispatcher.h"
#include "exports.h"
#include "filemanager.h"
#include "includes.h"
#include "parser.h"
#include "references.h"
#include "units.h"

#define MAX_LINE_LENGTH 3000


static CommandHandler(about_handler) {
    printf("This is Clefactory v0.0\n");
    return EXIT_SUCCESS;
}

static const char *about_help(void) { return ""; }

static CommandHandler(help_handler);

static DispatchTable dispatchTable[] = {
    {"about",      about_handler,      about_help     },
    {"exports",    exports_handler,    exports_help   },
    {"includes",   includes_handler,   includes_help  },
    {"references", references_handler, references_help},
    {"units",      units_handler,      units_help     },
    {"?",          help_handler,       NULL           }, /* NOTE: Help will stop at "?" */
    {"help",       help_handler,       NULL           },
    {"",           NULL,               NULL           }
};

static CommandHandler(help_handler) {
    for (DispatchTable *t = dispatchTable; strcmp(t->command, "?") != 0; t++)
        fprintf(stdout, "%s %s\n", t->command, t->help());
    return EXIT_SUCCESS;
}

void cli_repl(FileTable fileTable, CXIndex index) {
    char command[MAX_LINE_LENGTH];

    fprintf(stdout, "> ");
    while (fgets(command, sizeof(command), stdin) != NULL) {
        // TODO: ... so that we can refresh the index if needed,
        // before doing the operation
        int error_code = dispatch_command(index, fileTable, command, dispatchTable);
        if (error_code != 0) {
            fprintf(stderr, "Error: %d\n", error_code);
        }
        fprintf(stdout, "> ");
    }
}
