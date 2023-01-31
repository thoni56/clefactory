#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED

/* DISPATCHER - read commands from stdin, decode and dispatch until EOF */

#include <stdio.h>


typedef int CommandHandler(const char *);

typedef struct {
    const char *command;
    CommandHandler *handler;
} DispatchTable;

extern void dispatch_command(const char *command, DispatchTable *table);

#endif
