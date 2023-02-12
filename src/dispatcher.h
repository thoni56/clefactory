#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED

#include <stdio.h>

#include "clang-c/Index.h"

#include "filemanager.h"


/* Will receive a FileTable and an array of arguments both terminated by a NULL pointers. */
/* Callee have to duplicate arguments if they should be saved, will be free'd after return. */
#define CommandHandler(handler) int (handler)(CXIndex index, FileTable fileTable, const char *arguments[])

typedef struct {
    const char *command;
    CommandHandler(*handler);
    const char *(*help)(void);
} DispatchTable;

// Returns EXIT_FAILURE if no handler found, else return code from handler
extern int dispatch_command(CXIndex index, FileTable fileTable, const char *command, DispatchTable *table);

#endif
