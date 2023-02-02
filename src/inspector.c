#include "inspector.h"

#include "dispatcher.h"
#include <clang-c/Index.h>
#include <stdio.h>


int export_lister(const char *arguments) {
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit tu = clang_parseTranslationUnit(index, arguments, argv + 1, argc - 1,
                                                      0, 0, 0);
  if (!tu) {
    return 1;
  }

  CXCursor cursor = clang_getTranslationUnitCursor(tu);
  clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData client_data) {
    if (clang_getCursorKind(c) == CXCursor_VarDecl ||
        clang_getCursorKind(c) == CXCursor_FunctionDecl ||
        clang_getCursorKind(c) == CXCursor_TypedefDecl) {
      CXString symbol_name = clang_getCursorSpelling(c);
      fprintf(stdout, "Externally visible symbol: %s\n", clang_getCString(symbol_name));
      clang_disposeString(symbol_name);
    }
    return CXChildVisit_Continue;
  }, 0);

  clang_disposeTranslationUnit(tu);
  clang_disposeIndex(index);
  return 0;
}
