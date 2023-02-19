#include "json_adapter.h"
#include <cjson/cJSON.h>


extern cJSON *jsonParse(const char *json_string) {
    return cJSON_Parse(json_string);
}

extern cJSON *jsonGetObjectItem(cJSON *object, const char *const elementName) {
    return cJSON_GetObjectItem(object, elementName);
}

extern void jsonDelete(cJSON *object) {
    cJSON_Delete(object);
}
