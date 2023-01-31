#include <stdlib.h>
#include <stdio.h>

#include "dispatcher.h"


int main(int argc, char *argv[]) {
    dispatch_commands(stdin);
    return EXIT_SUCCESS;
}
