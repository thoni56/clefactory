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
    char *delimiter = "\r\n\r\n";
    int length = strlen(payload)+strlen(delimiter);
    char header[1000];
    snprintf(header, sizeof(header),
             "Content-Length: %d\r\nContent-type: application/vscode-jsonrpc;charset=utf-8\r\n\r\n", length);

    // Concatenate the header and message and delimiter
    int message_length = strlen(header) + strlen(payload) + strlen(delimiter);
    char *buffer = malloc(message_length);
    snprintf(buffer, message_length, "%s%s%s", header, payload, delimiter);
    fprintf(stderr, "buffer: %s", buffer);

    // Send the message
    int result = fputs(buffer, file);
    fflush(file);
    free(buffer);
    return result;
}
