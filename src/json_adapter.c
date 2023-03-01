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

char *jsonPrint(cJSON *object) { return cJSON_PrintUnformatted(object); }

void jsonDelete(cJSON *object) { cJSON_Delete(object); }

// Will return EOF on error, else non-negative value
int jsonSend(cJSON *json, FILE *file) {
    char *payload = jsonPrint(json);

    // Create the message header
    int length = strlen(payload)+4;
    char header[1000];
    snprintf(header, sizeof(header),
             "Content-Length: %d\r\nContent-type: application/vscode-jsonrpc;charset=utf-8\r\n\r\n", length);

    // Concatenate the header and message and delimiter
    char *delimiter = "\r\n\r\n";
    int message_length = strlen(header) + strlen(payload) + strlen(delimiter);
    char *buffer = malloc(message_length);
    strcpy(buffer, header);
    strcat(buffer, payload);
    strcat(buffer, delimiter);

    // Send the message
    int result = fputs(buffer, file);
    fflush(file);
    free(buffer);
    return result;
}
