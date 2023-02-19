#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "clang_adapter.h"

#include "filemanager.h"


extern int parse_files(FileTable fileTable, CXIndex index);

#endif
