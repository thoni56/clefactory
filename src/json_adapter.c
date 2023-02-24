#include "json_adapter.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"


cJSON *jsonParse(const char *json_string) { return cJSON_Parse(json_string); }

cJSON *jsonGetObjectItem(cJSON *object, const char *const elementName) {
    return cJSON_GetObjectItem(object, elementName);
}

void jsonDelete(cJSON *object) { cJSON_Delete(object); }
