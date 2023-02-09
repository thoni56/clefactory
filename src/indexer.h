#ifndef INDEXER_H_INCLUDED
#define INDEXER_H_INCLUDED

#include "clang_adaptor.h"


extern const char *indexer_help(void);
extern int indexer_handler(CXIndex index, const char *arguments[]);

#endif
