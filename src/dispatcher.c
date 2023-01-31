#include "dispatcher.h"


void dispatch_command(const char *command, DispatchTable *table) {
    table[0].handler("");
}
