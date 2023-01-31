#include <stdlib.h>
#include <stdio.h>

#include "dispatcher.h"

static DispatchTable dispatchTable[1];

int main(int argc, char *argv[]) {
    dispatch_commands(stdin, dispatchTable);
    return EXIT_SUCCESS;
}
