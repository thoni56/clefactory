#include "references.h"

#include <clang-c/Index.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "clang_adaptor.h"

int references_handler(const char *arguments[]) {
    CXIndex index = createIndex(0, 0);

    int result_code = EXIT_SUCCESS;

    disposeIndex(index);

    return result_code;
}
