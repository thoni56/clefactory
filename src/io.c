#include "io.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>


#ifdef __WIN32__
#define FILE_PATH_SEPARATOR '\\'
#else
#define FILE_PATH_SEPARATOR '/'
#endif


bool exists(char *path) {
    struct stat st;
    int rc = stat(path, &st);
    return rc == 0;
}

FILE *openFile(char *fileName, char *modes) {
    return fopen(fileName, modes);
}

int closeFile(FILE *file) {
    return fclose(file);
}

void createDirectory(char *dirname) {
    struct stat st;

    if (exists(dirname)) {
        stat(dirname, &st);
        if (S_ISDIR(st.st_mode))
            return;
        removeFile(dirname);
    }
#ifdef __WIN32__
    mkdir(dirname);
#else
    mkdir(dirname, 0777);
#endif
}


void recursivelyCreateFileDirIfNotExists(char *fpath) {
    char *p;
    int ch, len;
    bool loopFlag = true;

    /* Check each level from the deepest, stop when it exists */
    len = strlen(fpath);
    for (p=fpath+len; p>fpath && loopFlag; p--) {
        if (*p!=FILE_PATH_SEPARATOR)
            continue;
        ch = *p; *p = 0;        /* Truncate here, remember the char */
        if (directoryExists(fpath)) {
            loopFlag=false;
        }
        *p = ch;                /* Restore the char */
    }
    /* Create each of the remaining levels */
    for (p+=2; *p; p++) {
        if (*p!=FILE_PATH_SEPARATOR)
            continue;
        ch = *p; *p = 0;
        createDirectory(fpath);
        *p = ch;
    }
}


void removeFile(char *fileName) {
    unlink(fileName);
}

int fileStatus(char *path, struct stat *statP) {
    struct stat st;
    int return_value;

    return_value = stat(path, &st); /* Returns 0 on success */
    if (statP != NULL)
        *statP = st;
    return return_value;
}

time_t fileModificationTime(char *path) {
    struct stat st;
    if (fileStatus(path, &st) !=0)
        return 0;               /* File not found? */
    return st.st_mtime;
}

size_t fileSize(char *path) {
    struct stat st;
    if (fileStatus(path, &st) !=0)
        return 0;               /* File not found? */
    return st.st_size;
}

bool isDirectory(char *path) {
    return directoryExists(path);
}

bool directoryExists(char *path) {
    struct stat st;
    int statResult;

    statResult = stat(path, &st);
    return statResult==0 && S_ISDIR(st.st_mode);
}

bool fileExists(char *fullPath) {
    struct stat st;
    int statResult;

    statResult = stat(fullPath, &st);
    return statResult==0 && S_ISREG(st.st_mode);
}

size_t readFile(FILE *file, void *buffer, size_t size, size_t count) {
    return fread(buffer, size, count, file);
}

size_t writeFile(FILE *file, void *buffer, size_t size, size_t count) {
    return fwrite(buffer, size, count, file);
}

int readChar(FILE *file) {
    return getc(file);
}

char *getCwd(char *buffer, size_t size) {
    return getcwd(buffer, size);
}

char **getFilesInCurrentDirectory(void) {
    DIR *d;
    struct dirent *dir;
    char **files = malloc(sizeof(const char *));
    unsigned length = 1;
    files[length-1] = NULL;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            files = realloc(files, ++length*sizeof(const char *));
            files[length-2] = strdup(dir->d_name);
            files[length-1] = NULL;
        }
        closedir(d);
    }
    return files;
}

int writePipe(int pipe, const char *buffer, size_t length) {
    int rc = write(pipe, buffer, length);
    fsync(pipe);
    return rc;
}

int readPipe(int pipe, char *buffer, size_t size) {
    int rc = read(pipe, buffer, size);
    fsync(pipe);
    return rc;
}

char *readLine(char *buffer, size_t max_size, FILE *file) {
    return fgets(buffer, max_size, file);
}
