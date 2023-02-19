#ifndef FILEMANAGER_H_INCLUDED
#define FILEMANAGER_H_INCLUDED

#include "clang_adapter.h"


typedef struct {
    const char *fileName;
    CXTranslationUnit unit;
} FileItem;

typedef FileItem *FileTable;


extern unsigned fileTableLength(FileTable fileTable);
extern FileItem fileTableElement(FileTable fileTable, unsigned index);
extern FileTable getTranslationUnitsFromCurrentDirectory(void);
extern void freeFileTable(FileTable fileTable);

#endif
