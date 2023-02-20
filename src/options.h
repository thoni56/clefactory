#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED

#include "error.h"


typedef enum { NO_MODE, CLI_MODE, LSP_MODE } Mode;

typedef struct {
    Mode mode;
    char *lsp_server_path;
    char *logfile_path;
} Options;

extern Options options;

extern ResultCode decode_options(int argc, char *argv[]);

#endif
