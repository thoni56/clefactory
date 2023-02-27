#include <cgreen/cgreen.h>
#include <cgreen/constraint_syntax_helpers.h>
#include <cgreen/mocks.h>

#include "client_handler.h"
#include "log.h"

#include "io.mock"
#include "json_adapter.mock"


Describe(ClientHandler);
BeforeEach(ClientHandler) {
    log_set_level(LOG_FATAL);
}
AfterEach(ClientHandler) {}


Ensure(ClientHandler, can_compile_with_an_empty_test) {
    char *content_length_header = "Content-Length: 43\r\n";
    char *delimiter = "\r\n";

    expect(readLine, will_set_contents_of_parameter(buffer, content_length_header, strlen(content_length_header)),
           will_return(content_length_header));
    expect(readLine, will_set_contents_of_parameter(buffer, delimiter, strlen(delimiter)),
           will_return(content_length_header));

    handle_client_request(42);
}
