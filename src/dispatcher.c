#include "dispatcher.h"

#include <strings.h>


void dispatch_command(const char *command, DispatchTable *table) {
    if (strncmp(command, "about", 5) == 0)
        table[0].handler("");
    else
        table[1].handler("");
}
