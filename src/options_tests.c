#include "error.h"
#include "log.h"
#include "options.h"
#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>

Describe(Options);
BeforeEach(Options) {
    log_set_level(LOG_FATAL);
}
AfterEach(Options) {}

Ensure(Options, can_decode_lsp_option_with_argument) {
    char *argv[] = {
        "", "--lsp=path"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    assert_that(decode_options(argc, argv), is_equal_to(RC_OK));
    assert_that(options.lsp_server_path, is_equal_to_string("path"));
}

Ensure(Options, will_return_error_on_unknown_option) {
    char *argv[] = {
        "", "--sldfalsfkjalsfj"
    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    assert_that(decode_options(argc, argv), is_equal_to(RC_UNKNOWN_OPTION));
}

Ensure(Options, can_parse_two_options) {
    char *argv[] = {
        "", "--lsp=path", "--log=log"

    };
    int argc = sizeof(argv)/sizeof(argv[0]);

    assert_that(decode_options(argc, argv), is_equal_to(RC_OK));
    assert_that(options.lsp_server_path, is_equal_to_string("path"));
    assert_that(options.logfile_path, is_equal_to_string("log"));
}
