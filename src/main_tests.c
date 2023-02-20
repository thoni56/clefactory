#include "options.h"
#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

extern int main_(int argc, char *argv[]);

#include "clang_adapter.mock"
#include "filemanager.mock"
#include "repl.mock"
#include "lsp.mock"
#include "options.mock"


Describe(Main);
BeforeEach(Main) {}
AfterEach(Main) {}

Ensure(Main, will_inject_the_given_server) {
    char *argv[] = {"", "--lsp=server_name"};
    int argc = sizeof(argv)/sizeof(argv[0]);

    expect(decode_options);
    options.mode = LSP_MODE;
    options.lsp_server_path = "server_name";

    FileItem nullFileItem = {.fileName = NULL};
    FileTable fileTable = &nullFileItem;
    expect(getTranslationUnitsFromCurrentDirectory, will_return(fileTable));
    expect(createIndex, will_return(NULL));

    expect(lsp_inject, when(program_name, is_equal_to_string("server_name")));
    expect(lsp_repl);

    expect(disposeIndex);
    expect(freeFileTable);

    main_(argc, argv);
}
