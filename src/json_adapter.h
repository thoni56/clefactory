#ifndef JSON_ADAPTER_H_INCLUDED
#define JSON_ADAPTER_H_INCLUDED

#include "error.h"
#include <cjson/cJSON.h>


extern cJSON *jsonParse(const char * const json_string);

extern cJSON *jsonGetObjectItem(cJSON *object, const char *const elementName);

extern char *jsonPrint(cJSON *object);

extern void jsonDelete(cJSON *object);

extern ResultCode jsonSend(cJSON *json, int pipe);

#endif
