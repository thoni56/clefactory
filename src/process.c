#include "process.h"

#include <unistd.h>


pid_t fork_process(void) {
    return fork();
}
