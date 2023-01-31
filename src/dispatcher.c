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

    for (DispatchTable *t=table; t->handler != NULL; t++) {
        if (strcmp(command, t->command) == 0)
            return t->handler((const char **)arguments);
    }
    return EXIT_FAILURE;
}
