#include "dispatcher.h"

#include <stdlib.h>
#include <string.h>

static char **split_into_arguments(char *token) {
    char **arguments = malloc(sizeof(char *));
    int argc = 0;

    token = strtok(NULL, " ");
    while (token != NULL) {
        arguments = realloc(arguments, (argc+2) * sizeof(char *));
        arguments[argc++] = token;
        token = strtok(NULL, " ");
    }
    arguments[argc] = NULL;
    return arguments;
}

int dispatch_command(const char *line, DispatchTable *table) {
    char *command = strdup(line);
    command[strlen(command)-1] = '\0'; /* Remove newline */
    char *token = strtok(command, " "); /* Split off the command */

    char **arguments = split_into_arguments(token);

    for (DispatchTable *t=table; t->handler != NULL; t++) {
        if (strcmp(command, t->command) == 0)
            return t->handler((const char **)arguments);
    }
    return EXIT_FAILURE;
}
