#include "filemanager.h"

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "fileio.h"


#define FILE_TABLE_SIZE 1000


static bool endsWith(const char *fileName, const char *ending) {
    if (strlen(ending) > strlen(fileName))
        return false;
    return strcmp(&fileName[strlen(fileName)-strlen(ending)], ending) == 0;
}


unsigned fileTableLength(FileTable fileTable) {
    unsigned length = 0;
    for (FileTableElement *e = fileTable; e->fileName != NULL; e++)
        length++;
    return length;
}

FileTableElement fileTableElement(FileTable fileTable, unsigned index) {
    return fileTable[index];
}

FileTable getFilesFromCurrentDirectory(void) {
    const char **fileNameTable = getFilesInCurrentDirectory();
    // TODO: how long is the fileNameTable? Use that in allocation...

    FileTable fileTable = (FileTable)malloc(FILE_TABLE_SIZE*sizeof(FileTableElement));
    fileTable[0].fileName = NULL;

    int i = 0;
    for (const char **f = fileNameTable; *f != NULL; f++) {
        if (endsWith(*f, ".c")) {
            fileTable[i].fileName = *f;
            fileTable[++i].fileName = NULL;
        }
    }

    return fileTable;
}
