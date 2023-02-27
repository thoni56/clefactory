#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>
#include <string.h>

#include "client_handler.h"
#include "error.h"
#include "log.h"

#include "io.mock"
#include "json_adapter.mock"


Describe(ClientHandler);
BeforeEach(ClientHandler) {
    log_set_level(LOG_FATAL);
}
AfterEach(ClientHandler) {}


Ensure(ClientHandler, will_return_broken_input_for_broken_header) {
    char *content_length_header = "Content-Length: 43\r\n";

    // Header
    expect(readLine, will_set_contents_of_parameter(buffer, content_length_header, strlen(content_length_header)+1),
           will_return(content_length_header));
    expect(readLine, will_return(NULL));

    FILE *server_channel = (FILE *)0xcdcdcd;
    assert_that(handle_client_request(server_channel, stdin), is_equal_to(RC_BROKEN_INPUT_FROM_CLIENT));
}

static const char *delimiter = "\r\n";

static void expect_correct_header(void) {
    static const char *content_length_header = "Content-Length: 43\r\n";

    // Header
    expect(readLine,
           will_set_contents_of_parameter(buffer, content_length_header,
                                          strlen(content_length_header) + 1),
           will_return(content_length_header));
    expect(readLine, will_set_contents_of_parameter(buffer, delimiter, strlen(delimiter) + 1),
           will_return(delimiter));
}

Ensure(ClientHandler, will_return_broken_input_for_broken_payload) {
    expect_correct_header();

    // Payload
    expect(readLine, will_return(NULL));

    FILE *server_channel = (FILE *)0xcdcdcd;
    assert_that(handle_client_request(server_channel, stdin), is_equal_to(RC_BROKEN_INPUT_FROM_CLIENT));
}

Ensure(ClientHandler, will_send_received_one_line_json_request_to_server) {
    expect_correct_header();

    // One-line valid json
    const char *valid_json = "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\",\"params\":{}}";
    expect(readLine,
           will_set_contents_of_parameter(buffer, valid_json, strlen(valid_json)+1),
           will_return(valid_json));
    expect(readLine, will_set_contents_of_parameter(buffer, delimiter, strlen(delimiter)+1),
           will_return(delimiter));

    cJSON *root = (cJSON*)0xababab;
    expect(jsonParse, will_return(root));
    cJSON method = {.valuestring = "exit"};
    expect(jsonGetObjectItem, when(object, is_equal_to(root)),
           will_return(&method));

    FILE *server_channel = (FILE *)0xcdcdcd;
    expect(jsonSend, when(file, is_equal_to(server_channel)), when(object, is_equal_to(root)));
    expect(jsonDelete, when(object, is_equal_to(root)));

    assert_that(handle_client_request(server_channel, stdin), is_equal_to(RC_OK));
}
