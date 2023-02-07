#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED

#include <stdio.h>


/* Will receive an array of arguments terminated by a NULL pointer */
typedef int CommandHandler(const char *arguments[]);

typedef struct {
    const char *command;
    CommandHandler *handler;
    const char *(*help)(void);
} DispatchTable;

// Returns EXIT_FAILURE if no handler found, else return code from handler
extern int dispatch_command(const char *command, DispatchTable *table);

#endif
