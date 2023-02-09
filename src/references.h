#ifndef REFERENCES_H_INCLUDED
#define REFERENCES_H_INCLUDED

#include "clang_adaptor.h"


const char *references_help(void);

int references_handler(CXIndex index, const char *arguments[]);

#endif
