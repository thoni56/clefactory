#include <clang-c/Index.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  CXIndex index = clang_createIndex(0, 0);
  CXTranslationUnit tu = clang_parseTranslationUnit(index, 0, argv, argc, 0, 0,
                                                      CXTranslationUnit_None);
  if (!tu) {
    return 1;
  }

  unsigned int num_diagnostics = clang_getNumDiagnostics(tu);
  if (num_diagnostics) {
    for (unsigned i = 0; i < num_diagnostics; ++i) {
      CXDiagnostic diagnostic = clang_getDiagnostic(tu, i);
      CXString diagnostic_string = clang_formatDiagnostic(diagnostic,
                                                           clang_defaultDiagnosticDisplayOptions());
      fprintf(stderr, "%s\n", clang_getCString(diagnostic_string));
      clang_disposeString(diagnostic_string);
      clang_disposeDiagnostic(diagnostic);
    }
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
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
