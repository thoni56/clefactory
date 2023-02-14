#ifndef LSP_H_INCLUDED
#define LSP_H_INCLUDED

#include "filemanager.h"


extern void lsp_init(void);

extern int lsp_event_loop(FileTable fileTable, CXIndex index);

#endif
