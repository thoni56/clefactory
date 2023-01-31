#include "dispatcher.h"

#define MAX_LINE_LENGTH 3000

void dispatch_commands(FILE *channel, DispatchTable *table) {
    char line[MAX_LINE_LENGTH+1];
    while (fgets(line, MAX_LINE_LENGTH, channel) != 0)
        ;
}
