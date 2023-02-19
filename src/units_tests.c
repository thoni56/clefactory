#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include <stdlib.h>

#include "filemanager.h"
#include "units.h"

#include "clang_adapter.h"


Describe(Units);
BeforeEach(Units) {}
AfterEach(Units) {}


Ensure(Units, can_list_all_translation_units_in_an_empty_filetable) {
    FileItem item0 = {.fileName = NULL};
    FileTable fileTable = &item0;

    const char *empty_arguments[] = {NULL};

    units_handler(index, fileTable, empty_arguments);
}
