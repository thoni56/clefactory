#ifndef JSON_ADAPTER_H_INCLUDED
#define JSON_ADAPTER_H_INCLUDED

#include <stdio.h>
#include <cjson/cJSON.h>

#include "error.h"


extern cJSON *jsonParse(const char * const json_string);

extern cJSON *jsonGetObjectItem(cJSON *object, const char *const elementName);

extern char *jsonPrint(cJSON *object);

extern void jsonDelete(cJSON *object);

extern ResultCode jsonSend(cJSON *object, FILE *file);

#endif
