#ifndef JSON_ADAPTER_H_INCLUDED
#define JSON_ADAPTER_H_INCLUDED

#include <cjson/cJSON.h>


extern cJSON *jsonParse(const char * const json_string);

extern cJSON *jsonGetObjectItem(cJSON *object, const char *const elementName);

extern void jsonDelete(cJSON *object);

#endif
