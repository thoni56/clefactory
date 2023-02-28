#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_HEADER_FIELD_LEN 1000

static int lsp_parse_header(void) {
    char input[MAX_HEADER_FIELD_LEN];
    unsigned long content_length = 0;

    for (;;) {
        if (fgets(input, MAX_HEADER_FIELD_LEN, stdin) == NULL)
            return -1;
        fprintf(stderr, "Got '%s' while parsing header\n", input);
        if (strcmp(input, "\r\n") == 0) { // End of header
            if (content_length == 0)
                fprintf(stderr, "Dummy server received incomplete header\n");
            else
                return content_length;
        }

        char *buffer_part = strtok(input, " ");
        if (strcmp(buffer_part, "Content-Length:") == 0) {
            buffer_part = strtok(NULL, "\n");
            content_length = atoi(buffer_part);
        }
    }
}

static void send_json_rpc_message(char *payload) {
    // Create the message header
    int length = strlen(payload);
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
    fputs(buffer, stdout);
    free(buffer);
}


int main(int argc, char **argv) {
    char input[1000];
    FILE *responses = fopen("responses.json", "r");

    for (;;) {
        int content_length = lsp_parse_header();
        if (content_length == -1)
            break;
        if (fgets(input, content_length+1, stdin) == NULL)
            break;
        cJSON *root = cJSON_Parse(input);
        cJSON *method = cJSON_GetObjectItem(root, "method");
        if (method != NULL) {
            fprintf(stderr, "Dummy server received '%s' request\n", method->valuestring);
            if (strcmp(method->valuestring, "exit") == 0)
                return EXIT_SUCCESS;
            fgets(input, sizeof(input), stdin);
            if (strcmp(input, "\r\n") != 0) {
                fprintf(stderr, "Dummy server expected delimiter");
                return EXIT_FAILURE;
            }
            char response[1000];
            fgets(response, 1000, responses);
            send_json_rpc_message(response);
        } else {
            fprintf(stderr, "Dummy server received an invalid JSON-RPC message: '%s'\n", input);
        }
        cJSON_Delete(root);
    }
    return EXIT_FAILURE;
}
