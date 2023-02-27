#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char input[1000];

    for (;;) {
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        fprintf(stderr, "%s", input);
    }
    return EXIT_FAILURE;
}
