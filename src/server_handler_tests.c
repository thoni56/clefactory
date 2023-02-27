#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include "server_handler.h"
#include "log.h"

#include "io.mock"


Describe(ServerHandler);
BeforeEach(ServerHandler) {
    log_set_level(LOG_FATAL);
}
AfterEach(ServerHandler) {}


static char *create_json_message_from(const char *payload) {
    char *message = malloc(1000);
    sprintf(message, "Content-Length: %lu\r\nContent-type: application/vscode-jsonrpc;charset=utf-8\r\n\r\n%s\r\n",
            strlen(payload), payload);
    return message;
}

Ensure(ServerHandler, will_report_failed_read) {
    FILE *server_request_channel = (FILE *)0xececec;
    FILE *client_request_channel = (FILE *)0xededed;
    expect(readPipe, when(pipe, is_equal_to(server_request_channel)),
           will_return(-1));

    assert_that(handle_server_response(server_request_channel, client_request_channel), is_equal_to(RC_ERROR_RECEIVING_FROM_SERVER));
}


Ensure(ServerHandler, will_report_failed_writing_to_client) {
    FILE *server_request_channel = (FILE *)0xececec;
    FILE *client_request_channel = (FILE *)0xededed;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";
    char *input = create_json_message_from(payload);

    expect(readLine, when(file, is_equal_to(server_request_channel)),
           will_set_contents_of_parameter(buffer, input, strlen(input)),
           will_return(65));

    expect(writePipe, when(pipe, is_equal_to(client_request_channel)),
           will_return(-1));

    assert_that(handle_server_response(server_request_channel, client_request_channel),
                is_equal_to(RC_ERROR_SENDING_TO_CLIENT));
}

Ensure(ServerHandler, will_send_response_from_server_to_client) {
    FILE *server_request_channel = (FILE *)0xececec;
    FILE *client_request_channel = (FILE *)0xededed;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";
    char *input = create_json_message_from(payload);

    expect(readPipe, when(pipe, is_equal_to(server_request_channel)),
           will_set_contents_of_parameter(buffer, input, strlen(input)),
           will_return(65));

    expect(writePipe, when(pipe, is_equal_to(client_request_channel)), when(buffer, is_equal_to_string(input)),
           will_return(0));

    // Needs to be an exit request, otherwise the server handler will not return
    assert_that(handle_server_response(server_request_channel, client_request_channel), is_equal_to(RC_OK));
}
