#ifndef LSP_H_INCLUDED
#define LSP_H_INCLUDED

#include "filemanager.h"


extern void lsp_init(void);

extern int lsp_event_loop(FileTable fileTable, CXIndex index);

extern int lsp_listener(FileTable fileTable, CXIndex index, int client_input_pipe,
                     int client_output_pipe);

#endif
