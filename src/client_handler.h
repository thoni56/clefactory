#ifndef CLIENT_HANDLER_H_INCLUDED
#define CLIENT_HANDLER_H_INCLUDED

#include <stdio.h>
#include "error.h"

extern ResultCode handle_client_request(FILE *server_request_channel, FILE *client_request_pipe);

#endif
