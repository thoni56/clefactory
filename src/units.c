#include "units.h"

#include <stdlib.h>

#include "dispatcher.h"
#include "filemanager.h"


const char *units_help(void) {
    return "- print all translation units found in current directory";
}

CommandHandler(units_handler) {
    for (int i = 0; fileTable[i].fileName != NULL; i++)
        printf("%s\n", fileTable[i].fileName);
    return EXIT_SUCCESS;
}
