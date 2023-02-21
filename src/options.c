#include "options.h"

#include "error.h"
#include "log.h"
#include <string.h>

Options options = {
    .mode = NO_MODE,
    .logfile_path = NULL
};

ResultCode decode_options(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--cli") == 0) {
            options.mode = CLI_MODE;
        } else if (strncmp("--lsp=", argv[i], 6) == 0) {
            options.lsp_server_path = argv[i] + 6;
            options.mode = LSP_MODE;
        } else if (strncmp("--log=", argv[i], 6) == 0) {
            options.logfile_path = argv[i] + 6;
        } else {
            log_error("Unknown option: %s", argv[i]);
            return RC_UNKNOWN_OPTION;
        }
    }
    if (options.mode == NO_MODE) {
        log_error("No mode selected, select --cli or --lsp=<path>");
        return RC_NO_MODE_SELECTED;
    }
    return RC_OK;
}
