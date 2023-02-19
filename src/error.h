#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

typedef enum {
    RC_OK = 0,
    RC_PIPE_CREATION_ERROR = -100,
    RC_FORK_FAILED = -101,
} ResultCode;

#endif
