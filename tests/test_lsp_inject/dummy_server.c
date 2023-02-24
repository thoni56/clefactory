#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_HEADER_FIELD_LEN 1000

static unsigned long lsp_parse_header(void) {
    char buffer[MAX_HEADER_FIELD_LEN];
    unsigned long content_length = 0;

    for (;;) {
        fgets(buffer, MAX_HEADER_FIELD_LEN, stdin);
        if (strcmp(buffer, "\r\n") == 0) { // End of header
            if (content_length == 0)
                fprintf(stderr, "Dummy server received incomplete header\n");
            else
                return content_length;
        }

        char *buffer_part = strtok(buffer, " ");
        if (strcmp(buffer_part, "Content-Length:") == 0) {
            buffer_part = strtok(NULL, "\n");
            content_length = atoi(buffer_part);
        }
    }
}

int main(int argc, char **argv) {
    char input[1000];
    FILE *responses = fopen("responses.json", "r");

    while (fgets(input, sizeof(input), stdin)) {
        lsp_parse_header();
        cJSON *root = cJSON_Parse(input);
        cJSON *method = cJSON_GetObjectItem(root, "method");
        if (method != NULL) {
            if (strcmp(method->valuestring, "initialize") == 0) {
                fprintf(stderr, "Dummy server received an 'initialize' request\n");
            } else if (strcmp(method->valuestring, "shutdown") == 0) {
                fprintf(stderr, "Dummy server received a 'shutdown' request\n");
            } else if (strcmp(method->valuestring, "exit") == 0) {
                fprintf(stderr, "Dummy server received an 'exit' request\n");
                return EXIT_SUCCESS;
            } else {
                fprintf(stderr, "Dummy server received an unknown request with method '%s'\n",
                        method->valuestring);
            }
            char response[1000];
            fgets(response, 1000, responses);
            write(STDOUT_FILENO, response, strlen(response) - 1);
        } else {
            fprintf(stderr, "Dummy server received an invalid JSON-RPC message: '%s'\n", input);
        }
        cJSON_Delete(root);
    }
    return EXIT_FAILURE;
}
