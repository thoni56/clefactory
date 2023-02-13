#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "parser.h"

#include "clang_adaptor.mock"


Describe(Indexer);
BeforeEach(Indexer) {}
AfterEach(Indexer) {}


Ensure(Indexer, can_index_empty_list_of_files) {
    FileItem null_fileitem = {.fileName = NULL};
    FileTable emptyFileTable = &null_fileitem;
    assert_that(parse_files(emptyFileTable, index), is_equal_to(EXIT_SUCCESS));
}


Ensure(Indexer, can_index_a_single_file) {
    FileItem single_fileitem[] = {{.fileName = "test.c"},
                                          {.fileName = NULL}};
    FileTable singleFileTable = single_fileitem;

    CXTranslationUnit unit = (CXTranslationUnit)0xfefefefe;
    expect(parseTranslationUnit, when(source_filename, is_equal_to("test.c")),
           will_return(unit));

    assert_that(parse_files(singleFileTable, index), is_equal_to(EXIT_SUCCESS));
    assert_that(single_fileitem[0].unit, is_equal_to(unit));
}
