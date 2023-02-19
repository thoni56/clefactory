#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>


int main(int argc, char **argv) {
    char input[1000];
    while (fgets(input, sizeof(input), stdin)) {
        cJSON *root = cJSON_Parse(input);
        cJSON *method = cJSON_GetObjectItem(root, "method");
        if (method != NULL) {
            if (strcmp(method->valuestring, "initialize") == 0) {
                printf("Received an 'initialize' message\n");
            } else if (strcmp(method->valuestring, "shutdown") == 0) {
                printf("Received a 'shutdown' message\n");
            } else if (strcmp(method->valuestring, "exit") == 0) {
                printf("Received an 'exit' message\n");
                return EXIT_SUCCESS;
            } else {
                printf("Received an unknown message with method '%s'\n", method->valuestring);
            }
        } else {
            printf("Received an invalid JSON-RPC message\n");
        }
        cJSON_Delete(root);
    }
    return EXIT_FAILURE;
}
