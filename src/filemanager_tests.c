#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "filemanager.h"

#include "fileio.mock"



Describe(Filemananger);
BeforeEach(Filemananger) {}
AfterEach(Filemananger) {}


Ensure(Filemananger, will_create_an_empty_filetable_for_an_empty_directory_without_compile_commands) {
    const char *emptyFileNameList[] = {NULL};
    expect(getFilesInCurrentDirectory, will_return(emptyFileNameList));

    FileTable fileTable = getFilesFromCurrentDirectory();

    assert_that(fileTableLength(fileTable), is_equal_to(0));
}

Ensure(Filemananger, will_create_an_empty_filetable_for_a_directory_without_c_files_and_compile_commands) {
    const char *fileNameListWithNoCFiles[] = {"file.x", NULL};
    expect(getFilesInCurrentDirectory, will_return(fileNameListWithNoCFiles));

    FileTable fileTable = getFilesFromCurrentDirectory();

    assert_that(fileTableLength(fileTable), is_equal_to(0));
}

Ensure(Filemananger, will_create_a_filetable_with_one_entry_for_a_directory_without_compile_commands_but_containing_one_c_file) {
    const char *singleCFileNameList[] = {"name.c", NULL};
    expect(getFilesInCurrentDirectory, will_return(singleCFileNameList));

    FileTable fileTable = getFilesFromCurrentDirectory();

    assert_that(fileTableLength(fileTable), is_equal_to(1));
    assert_that(fileTableElement(fileTable, 0).fileName, is_equal_to_string("name.c"));
}
