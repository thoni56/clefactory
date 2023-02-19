#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char input[1000];
    const char exit_request[] = "{\"jsonrpc\":\"2.0\",\"id\": 1,\"method\": \"exit\"}";
    while (fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "server received: '%s'\n", input);
        if (strcmp(input, exit_request) == 0)
            break;
    }
    return EXIT_SUCCESS;
}
