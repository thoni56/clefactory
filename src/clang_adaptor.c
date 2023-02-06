#include "clang_adaptor.h"
#include "clang-c/CXString.h"
#include "clang-c/Index.h"

CXIndex createIndex(int excludeDeclarationsFromPCH, int displayDiagnostics) {
    return clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
}

CXTranslationUnit parseTranslationUnit(CXIndex index, const char *source_filename,
                                       const char *const *command_line_args,
                                       int num_command_line_args,
                                       struct CXUnsavedFile *unsaved_files,
                                       unsigned num_unsaved_files, unsigned options) {
    return clang_parseTranslationUnit(index, source_filename, command_line_args,
                                      num_command_line_args, unsaved_files, num_unsaved_files,
                                      options);
}

unsigned int visitChildren(CXCursor parent, CXCursorVisitor visitor, CXClientData client_data) {
    return clang_visitChildren(parent, visitor, client_data);
}

CXCursor getTranslationUnitCursor(CXTranslationUnit unit) {
    return clang_getTranslationUnitCursor(unit);
}

void getInclusions(CXTranslationUnit tu, CXInclusionVisitor visitor, CXClientData client_data) {
    clang_getInclusions(tu, visitor, client_data);
}


CXFile getFile(CXTranslationUnit unit, const char *file_name) {
    return clang_getFile(unit, file_name);
}

CXString getFileName(CXFile file) {
    return clang_getFileName(file);
}

CXFile getIncludedFile(CXCursor cursor) {
    return clang_getIncludedFile(cursor);
}


unsigned int getNumDiagnostics(CXTranslationUnit unit) {
    return clang_getNumDiagnostics(unit);
}

CXDiagnostic getDiagnostic(CXTranslationUnit unit, unsigned int index) {
    return clang_getDiagnostic(unit, index);
}

CXSourceLocation getDiagnosticLocation(CXDiagnostic diagnostic) {
    return clang_getDiagnosticLocation(diagnostic);
}

void disposeDiagnostic(CXDiagnostic diagnostic) {
    return clang_disposeDiagnostic(diagnostic);
}


CXString getCursorSpelling(CXCursor cursor) { return clang_getCursorSpelling(cursor); }

enum CXCursorKind getCursorKind(CXCursor cursor) { return clang_getCursorKind(cursor); }

CXSourceLocation getCursorLocation(CXCursor cursor) { return clang_getCursorLocation(cursor); }

void getSpellingLocation(CXSourceLocation location, CXFile *file, unsigned int *line,
                         unsigned int *column, unsigned int *offset) {
    clang_getSpellingLocation(location, file, line, column, offset);
}

enum CX_StorageClass cursor_getStorageClass(CXCursor cursor) {
    return clang_Cursor_getStorageClass(cursor);
}

const char *getCString(CXString string) { return clang_getCString(string); }

int file_isEqual(CXFile file1, CXFile file2) { return clang_File_isEqual(file1, file2); }

void disposeString(CXString string) { clang_disposeString(string); }

enum CXLinkageKind getCursorLinkage(CXCursor cursor) { return clang_getCursorLinkage(cursor); }

void disposeIndex(CXIndex index) { clang_disposeIndex(index); }

void disposeTranslationUnit(CXTranslationUnit unit) { clang_disposeTranslationUnit(unit); }
