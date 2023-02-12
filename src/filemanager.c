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

FileTable getTranslationUnitsFromCurrentDirectory(void) {
    char **fileNameTable = getFilesInCurrentDirectory();

    int length = 0;
    for (unsigned i = 0; fileNameTable[i] != NULL; i++)
        length++;

    FileTable fileTable = (FileTable)malloc((length+1)*sizeof(FileTableElement));
    fileTable[0].fileName = NULL;

    int i = 0;
    for (char **f = fileNameTable; *f != NULL; f++) {
        if (endsWith(*f, ".c")) {
            fileTable[i].fileName = strdup(*f);
            fileTable[++i].fileName = NULL;
        }
    }
    for (int i = 0; fileNameTable[i] != NULL; i++)
        free(fileNameTable[i]);
    free(fileNameTable);
    return fileTable;
}

void freeFileTable(FileTable fileTable) {
    for (int i = 0; fileTable[i].fileName != NULL; i++)
        free((char *)fileTable[i].fileName);
    free(fileTable);
}
