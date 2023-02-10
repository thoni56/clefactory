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

    assert_that(fileTable, is_null);
}
