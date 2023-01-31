#include <stdlib.h>
#include <stdio.h>

#include "dispatcher.h"

#define MAX_LINE_LENGTH 3000


static DispatchTable dispatchTable[1];

int main(int argc, char *argv[]) {
    char command[MAX_LINE_LENGTH];
    while (fgets(command, sizeof(command), stdin) != NULL) {
        dispatch_command(command, dispatchTable);
    }
    return EXIT_SUCCESS;
}
