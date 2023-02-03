#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dispatcher.h"
#include "exports.h"
#include "indexer.h"

#define MAX_LINE_LENGTH 3000

static int about_handler(const char *arguments[]) {
    printf("This is Clefactory v0.0\n");
    return EXIT_SUCCESS;
}

static DispatchTable dispatchTable[] = {
    {"about", about_handler},
    {"exports", exports_handler},
    {"indexer", indexer_handler},
    {"", NULL}
};

int main(int argc, char *argv[]) {
    char command[MAX_LINE_LENGTH];
    fprintf(stdout, "> ");
    while (fgets(command, sizeof(command), stdin) != NULL) {
        if (strncmp(command, "?", 1) == 0)
            for (DispatchTable *t = dispatchTable; t->handler != NULL; t++)
                fprintf(stderr, "%s\n", t->command);
        else {
            int error_code = dispatch_command(command, dispatchTable);
            if (error_code != 0) {
                return error_code;
            }
        }
        fprintf(stdout, "> ");
    }
    return EXIT_SUCCESS;
}
