#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include "error.h"
#include "log.h"
#include "server_handler.h"

#include "io.mock"
#include "json_adapter.mock"

Describe(ServerHandler);
BeforeEach(ServerHandler) { log_set_level(LOG_FATAL); }
AfterEach(ServerHandler) {}

Ensure(ServerHandler, will_report_failed_receiving_from_client) {
    FILE *server_response_channel = (FILE *)0xececec;
    FILE *client_response_channel = (FILE *)0xededed;

    expect(readLine, when(file, is_equal_to(server_response_channel)), will_return(NULL));

    assert_that(handle_server_response(server_response_channel, client_response_channel),
                is_equal_to(RC_BROKEN_INPUT_CHANNEL_FROM_SERVER));
}

static char *create_json_content_length_for(const char *payload) {
    static char message[1000];
    sprintf(message, "Content-Length: %lu\r\n", strlen(payload));
    return message;
}

static char *type_header = "Content-type: application/vscode-jsonrpc;charset=utf-8\r\n";
static char *delimiter = "\r\n";

static void expect_header(FILE *server_response_channel, char *payload) {
    char *length_header = create_json_content_length_for(payload);

    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, length_header, strlen(length_header) + 1),
           will_return(!NULL));
    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, type_header, strlen(type_header) + 1),
           will_return(!NULL));
    expect(readLine, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, delimiter, strlen(delimiter) + 1),
           will_return(!NULL));
}

Ensure(ServerHandler, will_report_failed_sending_to_client) {
    FILE *server_response_channel = (FILE *)0xececec;
    FILE *client_response_channel = (FILE *)0xededed;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";

    expect_header(server_response_channel, payload);

    expect(readFile, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, payload, strlen(payload) + 1),
           will_return(strlen(payload)));

    cJSON root;
    expect(jsonParse, will_return(&root));

    /* Does it have an id? */
    cJSON id = {.valueint = 1};
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),  when(elementName, is_equal_to_string("id")),
           will_return(&id));
    /* What is it? */
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),  when(elementName, is_equal_to_string("id")),
           will_return(&id));

    cJSON result = {.valuestring = "exit"};
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),  when(elementName, is_equal_to_string("result")),
           will_return(&result));

    expect(jsonSend, when(file, is_equal_to(client_response_channel)), will_return(EOF));

    expect(jsonDelete, when(object, is_equal_to(&root)));

    assert_that(handle_server_response(server_response_channel, client_response_channel),
                is_equal_to(RC_ERROR_SENDING_TO_CLIENT));
}

Ensure(ServerHandler, will_send_response_from_server_to_client) {
    FILE *server_response_channel = (FILE *)0xececec;
    FILE *client_response_channel = (FILE *)0xededed;
    char *payload = "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"exit\"}";

    expect_header(server_response_channel, payload);

    expect(readFile, when(file, is_equal_to_hex(server_response_channel)),
           will_set_contents_of_parameter(buffer, payload, strlen(payload)), will_return(strlen(payload)));

    cJSON root;
    expect(jsonParse, will_return(&root));

    cJSON id = {.valueint = 1};
    /* Does it have an id? */
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),  when(elementName, is_equal_to_string("id")),
           will_return(&id));
    /* What is it? */
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),  when(elementName, is_equal_to_string("id")),
           will_return(&id));

    cJSON result = {.valuestring = "exit"};
    expect(jsonGetObjectItem, when(object, is_equal_to(&root)),  when(elementName, is_equal_to_string("result")),
           will_return(&result));

    expect(jsonSend, when(file, is_equal_to(client_response_channel)), will_return(0));

    expect(jsonDelete, when(object, is_equal_to(&root)));

    // Needs to be an exit response, otherwise the server handler will not return
    assert_that(handle_server_response(server_response_channel, client_response_channel),
                is_equal_to(RC_OK));
}
