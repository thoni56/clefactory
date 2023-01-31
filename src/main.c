#include <stdio.h>
#include <stdlib.h>

#include "dispatcher.h"

#define MAX_LINE_LENGTH 3000

static DispatchTable dispatchTable[1];

int main(int argc, char *argv[]) {
    char command[MAX_LINE_LENGTH];
    while (fgets(command, sizeof(command), stdin) != NULL) {
        int error_code = dispatch_command(command, dispatchTable);
        if (error_code)
            return error_code;
    }
    return EXIT_SUCCESS;
}
