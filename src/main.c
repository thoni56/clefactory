#include <stdlib.h>
#include <stdio.h>

#include "dispatcher.h"


int main(int argc, char *argv[]) {
    read_and_execute_commands(stdin);
    return EXIT_SUCCESS;
}
