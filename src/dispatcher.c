#include "dispatcher.h"

#include <string.h>


void dispatch_command(const char *command, DispatchTable *table) {
    char *copy = strdup(command);
    copy[strlen(copy)-1] = '\0';

    static char *arguments[2];
    arguments[0] = &copy[6];
    arguments[1] = NULL;

    if (strncmp(command, "about", 5) == 0)
        table[0].handler((const char **)arguments);
    else
        table[1].handler((const char **)arguments);
}
