#ifndef LSP_H_INCLUDED
#define LSP_H_INCLUDED

#include "error.h"
#include "filemanager.h"

extern ResultCode lsp_inject(const char *program_name, int input_pipe[], int output_pipe[]);

extern ResultCode lsp_repl(int server_request_pipe, int server_response_pipe, FileTable fileTable,
                           CXIndex index);

#endif
