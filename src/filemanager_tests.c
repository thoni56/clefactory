#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "filemanager.h"

#include "fileio.mock"



Describe(Filemananger);
BeforeEach(Filemananger) {}
AfterEach(Filemananger) {}


Ensure(Filemananger, will_create_an_empty_filetable_for_an_empty_directory) {
    const char *fileNameList[] = {NULL};
    expect(getFilesInCurrentDirectory, will_return(fileNameList));
    FileTable fileTable = getFilesFromCompilationDatabase();
    assert_that(fileTable, is_null);
}
