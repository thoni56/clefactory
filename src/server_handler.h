#ifndef SERVER_HANDLER_H_INCLUDED
#define SERVER_HANDLER_H_INCLUDED

#include "error.h"

extern void launch_server(int input_pipe, int output_pipe, const char program[]);

extern ResultCode handle_server_response(int server_response_pipe, int client_response_pipe);

#endif
