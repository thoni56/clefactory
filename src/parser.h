#ifndef INDEXER_H_INCLUDED
#define INDEXER_H_INCLUDED

#include "clang_adaptor.h"

#include "dispatcher.h"
#include "filemanager.h"


extern int parse_files(FileTable fileTable, CXIndex index);

extern const char *parser_help(void);
extern CommandHandler(parser_handler);

#endif
