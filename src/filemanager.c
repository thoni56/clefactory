#include "filemanager.h"

#include <stdlib.h>

#include "common.h"
#include "fileio.h"


FileTable getFilesFromCompilationDatabase(void) {
    const char **fileNameTable = getFilesInCurrentDirectory();
    UNUSED fileNameTable;

    FileTable fileTable = (FileTable)malloc(sizeof(FileTableElement));
    UNUSED fileTable;

    return NULL;
}
