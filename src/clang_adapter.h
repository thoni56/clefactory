#ifndef CLANG_ADAPTOR_H_INCLUDED
#define CLANG_ADAPTOR_H_INCLUDED

#include "clang-c/Index.h"

extern CXIndex createIndex(int excludeDeclarationsFromPCH, int displayDiagnostics);

extern CXTranslationUnit parseTranslationUnit(CXIndex index, const char *source_filename,
                                              const char *const *command_line_args,
                                              int num_command_line_args,
                                              struct CXUnsavedFile *unsaved_files,
                                              unsigned num_unsaved_files, unsigned options);

extern unsigned int visitChildren(CXCursor parent, CXCursorVisitor visitor,
                                  CXClientData client_data);

extern CXCursor getTranslationUnitCursor(CXTranslationUnit unit);
extern void getInclusions(CXTranslationUnit tu, CXInclusionVisitor visitor,
                          CXClientData client_data);
extern CXResult findReferencesInFile(CXCursor cursor, CXFile file, CXCursorAndRangeVisitor visitor);

extern CXFile getFile(CXTranslationUnit unit, const char *file_name);
extern CXString getFileName(CXFile file);

extern CXFile getIncludedFile(CXCursor cursor);

extern CXSourceLocation indexLoc_getCXSourceLocation(CXIdxLoc loc);
extern void getExpansionLocation(CXSourceLocation location, CXFile *file, unsigned int *line,
                                 unsigned int *column, unsigned int *offset);
extern CXSourceLocation getLocation(CXTranslationUnit tu, CXFile file, unsigned int line,
                                    unsigned int column);
extern CXSourceLocation getRangeStart(CXSourceRange range);
extern CXSourceLocation getRangeEnd(CXSourceRange range);

extern unsigned int getNumDiagnostics(CXTranslationUnit unit);
extern CXDiagnostic getDiagnostic(CXTranslationUnit unit, unsigned int index);
extern CXSourceLocation getDiagnosticLocation(CXDiagnostic diagnostic);
extern void disposeDiagnostic(CXDiagnostic diagnostic);

extern int cursor_isNull(CXCursor cursor);
extern CXCursor getCursor(CXTranslationUnit unit, CXSourceLocation location);
extern CXCursor getCursorDefinition(CXCursor cursor);
extern CXCursor getCursorReferenced(CXCursor cursor);
extern enum CXLinkageKind getCursorLinkage(CXCursor cursor);
extern CXSourceRange getCursorExtent(CXCursor cursor);
extern CXString getCursorSpelling(CXCursor);
extern enum CXCursorKind getCursorKind(CXCursor);
extern enum CX_StorageClass cursor_getStorageClass(CXCursor cursor);
extern CXSourceLocation getCursorLocation(CXCursor cursor);
extern void getSpellingLocation(CXSourceLocation location, CXFile *file, unsigned int *line,
                                unsigned int *column, unsigned int *offset);
extern const char *getCString(CXString string);
extern void disposeString(CXString string);

extern int file_isEqual(CXFile file1, CXFile file2);
extern void disposeIndex(CXIndex index);
extern void disposeTranslationUnit(CXTranslationUnit unit);

#endif
