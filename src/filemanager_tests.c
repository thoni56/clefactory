#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "filemanager.h"

#include "io.mock"



Describe(Filemananger);
BeforeEach(Filemananger) {}
AfterEach(Filemananger) {}


Ensure(Filemananger, will_create_an_empty_filetable_for_an_empty_directory_without_compile_commands) {
    const char **emptyFileNameList = malloc(sizeof(char *));
    emptyFileNameList[0] = NULL;
    expect(getFilesInCurrentDirectory, will_return(emptyFileNameList));

    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    assert_that(fileTableLength(fileTable), is_equal_to(0));
}

Ensure(Filemananger, will_create_an_empty_filetable_for_a_directory_without_c_files_and_compile_commands) {
    const char **fileNameListWithNoCFiles = malloc(2*sizeof(char *));
    fileNameListWithNoCFiles[0] = strdup("file.x");
    fileNameListWithNoCFiles[1] = NULL;
    expect(getFilesInCurrentDirectory, will_return(fileNameListWithNoCFiles));

    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    assert_that(fileTableLength(fileTable), is_equal_to(0));
}

Ensure(Filemananger, will_create_a_filetable_with_one_entry_for_a_directory_without_compile_commands_but_containing_one_c_file) {
    const char **singleCFileNameList = malloc(sizeof(char *));
    singleCFileNameList[0] = strdup("name.c");
    singleCFileNameList[1] = NULL;
    expect(getFilesInCurrentDirectory, will_return(singleCFileNameList));

    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    assert_that(fileTableLength(fileTable), is_equal_to(1));
    assert_that(fileTableElement(fileTable, 0).fileName, is_equal_to_string("name.c"));
}

Ensure(Filemananger, can_free_a_returned_filetable) {
    const char **singleCFileNameList = malloc(sizeof(char *));
    singleCFileNameList[0] = strdup("name.c");
    singleCFileNameList[1] = NULL;
    expect(getFilesInCurrentDirectory, will_return(singleCFileNameList));

    FileTable fileTable = getTranslationUnitsFromCurrentDirectory();

    freeFileTable(fileTable);
}
