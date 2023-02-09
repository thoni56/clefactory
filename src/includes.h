#ifndef INCLUDES_H_INCLUDED
#define INCLUDES_H_INCLUDED

#include "clang_adaptor.h"


extern const char *includes_help(void);
extern int includes_handler(CXIndex index, const char *arguments[]);

#endif
