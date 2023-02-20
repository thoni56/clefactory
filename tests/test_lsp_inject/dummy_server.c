#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <unistd.h>
#include <sys/stat.h>


int main(int argc, char **argv) {
    char input[1000];
    FILE *responses = fopen("responses.json", "r");

    while (fgets(input, sizeof(input), stdin)) {
        cJSON *root = cJSON_Parse(input);
        cJSON *method = cJSON_GetObjectItem(root, "method");
        if (method != NULL) {
            if (strcmp(method->valuestring, "initialize") == 0) {
                fprintf(stderr, "Dummy server received an 'initialize' message\n");
            } else if (strcmp(method->valuestring, "shutdown") == 0) {
                fprintf(stderr, "Dummy server received a 'shutdown' message\n");
            } else if (strcmp(method->valuestring, "exit") == 0) {
                fprintf(stderr, "Dummy server received an 'exit' message\n");
                return EXIT_SUCCESS;
            } else {
                fprintf(stderr, "Dummy server received an unknown message with method '%s'\n", method->valuestring);
            }
            char response[1000];
            fgets(response, 1000, responses);
            write(1, response, strlen(response)-1);
        } else {
            fprintf(stderr, "Dummy server received an invalid JSON-RPC message: '%s'\n", input);
        }
        cJSON_Delete(root);
    }
    return EXIT_FAILURE;
}
