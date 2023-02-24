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
    sprintf(message, "Content-Length: %lu %s", strlen(payload), payload);
    return message;
}

Ensure(ServerHandler, will_report_failed_read) {
    int server_pipe = 14;
    int client_pipe = 15;
    expect(readPipe, when(pipe, is_equal_to(server_pipe)),
           will_return(-1));

    assert_that(handle_server_response(server_pipe, client_pipe), is_equal_to(RC_ERROR_RECEIVING_FROM_SERVER));
}


Ensure(ServerHandler, will_report_failed_writing_to_client) {
    int server_pipe = 14;
    int client_pipe = 15;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";
    char *input = create_json_message_from(payload);

    expect(readPipe, when(pipe, is_equal_to(server_pipe)),
           will_set_contents_of_parameter(buffer, input, strlen(input)),
           will_return(65));

    expect(writePipe, when(pipe, is_equal_to(client_pipe)),
           will_return(-1));

    assert_that(handle_server_response(server_pipe, client_pipe), is_equal_to(RC_ERROR_SENDING_TO_CLIENT));
}
