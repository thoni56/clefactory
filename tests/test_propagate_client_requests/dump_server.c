#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *dump_file = fopen("dump", "w");
    char input[10000];

    for (;;) {
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;
        fprintf(dump_file, "%s", input);
    }
    return EXIT_FAILURE;
}
