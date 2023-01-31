#include "dispatcher.h"

#include <stdlib.h>
#include <string.h>


int dispatch_command(const char *line, DispatchTable *table) {
    char *command = strdup(line);
    command[strlen(command)-1] = '\0';
    strtok(command, " ");

    static char *arguments[2];
    arguments[0] = &command[6];
    arguments[1] = NULL;

    for (int i=0; table[i].handler != NULL; i++) {
        if (strcmp(command, table[i].command) == 0)
            return table[i].handler((const char **)arguments);
    }
    return EXIT_FAILURE;
}
