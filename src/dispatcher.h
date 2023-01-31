#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED

/* DISPATCHER - read commands from stdin, decode and dispatch until EOF */

#include <stdio.h>

extern void dispatch_commands(FILE *channel);

#endif
