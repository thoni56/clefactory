#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include "clang_adaptor.h"


typedef struct {
    const char *fileName;
} FileTableElement;

typedef FileTableElement *FileTable;


extern unsigned fileTableLength(FileTable fileTable);
extern FileTableElement fileTableElement(FileTable fileTable, unsigned index);
extern FileTable getFilesFromCurrentDirectory(void);

#endif
