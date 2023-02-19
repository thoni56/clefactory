#ifndef LSP_H_INCLUDED
#define LSP_H_INCLUDED

#include "filemanager.h"


extern void lsp_init(void);

extern int lsp_inject(const char *program_name, int input_pipe[], int output_pipe[]);

extern int lsp_listener(FileTable fileTable, CXIndex index, int client_input_pipe,
                     int client_output_pipe);

#endif
