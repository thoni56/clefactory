#ifndef INDEXER_H_INCLUDED
#define INDEXER_H_INCLUDED

#include "clang_adaptor.h"

#include "dispatcher.h"
#include "filemanager.h"


extern int indexFiles(FileTable fileTable, CXIndex index);

extern const char *indexer_help(void);
extern CommandHandler(indexer_handler);

#endif
