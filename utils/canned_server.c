#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>


static void send_json_rpc_message(char *payload) {
    // Create the message header
    int length = strlen(payload);
    char header[10000];
    snprintf(header, sizeof(header),
             "Content-Length: %d\r\nContent-type: application/vscode-jsonrpc;charset=utf-8\r\n\r\n", length);

    // Concatenate the header and message (and delimiter - not sent by clangd)
    int message_length = strlen(header) + strlen(payload) + 1;
    char *buffer = malloc(message_length);
    strcpy(buffer, header);
    strcat(buffer, payload);

    // Send the message
    fputs(buffer, stdout);
    free(buffer);
}


int main(int argc, char **argv) {
    sleep(1);
    FILE *responses = fopen("responses.json", "r");

    for (;;) {
        usleep(100);
        char response[10000];
        if (fgets(response, sizeof(response), responses) != NULL) {
            response[strlen(response)-1] = '\0';
            send_json_rpc_message(response);
        } else
            return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
