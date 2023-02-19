#ifndef REPL_H_INCLUDED
#define REPL_H_INCLUDED

#include "clang_adapter.h"
#include "filemanager.h"


extern void cli_repl(FileTable fileTable, CXIndex index);

#endif
