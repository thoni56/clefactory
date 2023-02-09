#ifndef EXPORTS_H_INCLUDED
#define EXPORTS_H_INCLUDED

#include "clang-c/Index.h"


extern const char *exports_help(void);
extern int exports_handler(CXIndex index, const char *arguments[]);

#endif
