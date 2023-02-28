#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include "server_handler.h"
#include "log.h"

#include "io.mock"
#include "json_adapter.mock"


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

Ensure(ServerHandler, will_report_failed_receiving_from_client) {
    FILE *server_response_channel = (FILE *)0xececec;
    FILE *client_response_channel = (FILE *)0xededed;
    expect(readLine, when(file, is_equal_to(server_response_channel)),
           will_return(NULL));

    assert_that(handle_server_response(server_response_channel, client_response_channel), is_equal_to(RC_BROKEN_INPUT_CHANNEL_FROM_SERVER));
}


Ensure(ServerHandler, will_report_failed_sending_to_client) {
    FILE *server_response_channel = (FILE *)0xececec;
    FILE *client_response_channel = (FILE *)0xededed;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";
    char *message = create_json_message_from(payload);

    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, message, strlen(message)),
           will_return(!NULL));
    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, "\r\n", 3),
           will_return(!NULL));

    cJSON root;
    cJSON method = {.valuestring = "exit"};
    expect(jsonParse, will_return(&root));
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),
           will_return(&method));

    expect(jsonSend, when(file, is_equal_to(client_response_channel)),
           will_return(EOF));

    expect(jsonDelete, when(object, is_equal_to(&root)));

    assert_that(handle_server_response(server_response_channel, client_response_channel),
                is_equal_to(RC_ERROR_SENDING_TO_CLIENT));
}

Ensure(ServerHandler, will_send_response_from_server_to_client) {
    FILE *server_response_channel = (FILE *)0xececec;
    FILE *client_response_channel = (FILE *)0xededed;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";
    char *message = create_json_message_from(payload);

    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, message, strlen(message)),
           will_return(!NULL));
    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, "\r\n", 3),
           will_return(!NULL));

    cJSON root;
    cJSON method = {.valuestring = "exit"};
    expect(jsonParse, will_return(&root));
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),
           will_return(&method));

    expect(jsonSend, when(file, is_equal_to(client_response_channel)),
           will_return(0));

    expect(jsonDelete, when(object, is_equal_to(&root)));

    // Needs to be an exit response, otherwise the server handler will not return
    assert_that(handle_server_response(server_response_channel, client_response_channel), is_equal_to(RC_OK));
}
