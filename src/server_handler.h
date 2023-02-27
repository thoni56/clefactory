#ifndef SERVER_HANDLER_H_INCLUDED
#define SERVER_HANDLER_H_INCLUDED

#include <stdio.h>
#include "error.h"


extern void launch_server(int input_pipe, int output_pipe, const char program[]);

extern ResultCode handle_server_response(FILE *server_response_channel, FILE *client_response_channel);

#endif
