#include "units.h"

#include <stdlib.h>

#include "dispatcher.h"
#include "filemanager.h"


CommandHandler(units_handler) {
    for (int i = 0; fileTable[i].fileName != NULL; i++)
        printf("%s\n", fileTable[i].fileName);
    return EXIT_SUCCESS;
}
