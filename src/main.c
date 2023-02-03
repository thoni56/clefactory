#include <stdio.h>
#include <stdlib.h>

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
    {"indexer", indexer_handler}
};

int main(int argc, char *argv[]) {
    char command[MAX_LINE_LENGTH];
    while (fgets(command, sizeof(command), stdin) != NULL) {
        int error_code = dispatch_command(command, dispatchTable);
        if (error_code != 0) {
            return error_code;
        }
    }
    return EXIT_SUCCESS;
}
