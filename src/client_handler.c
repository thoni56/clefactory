#include "client_handler.h"

#include "io.h"
#include "json_adapter.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 10000

static ResultCode parseRpcHeader(FILE *client_request_channel, int *content_length) {
    int length = 0;
    ResultCode rc = RC_OK;
    for (;;) {
        char input[BUFFER_SIZE];
        if (readLine(input, sizeof(input), client_request_channel) != NULL) {
            if (strcmp(input, "\r\n") == 0) { // End of header
                if (length == 0) {
                    log_error("Client sent incomplete header");
                    rc = RC_CLIENT_SENT_INCOMPLETE_HEADER;
                }
                break;
            }
            char *buffer_part = strtok(input, " ");
            if (strcmp(buffer_part, "Content-Length:") == 0) {
                buffer_part = strtok(NULL, "\n");
                length = atoi(buffer_part);
            }

        } else {
            log_error("Broken input channel from client");
            rc = RC_BROKEN_INPUT_CHANNEL_FROM_CLIENT;
            break;
        }
    }
    *content_length = length;
    return rc;
}

ResultCode handle_client_request(FILE *server_request_channel, FILE *client_request_channel) {
    int length;
    ResultCode rc;

    rc = parseRpcHeader(client_request_channel, &length);
    if (rc != RC_OK)
        return rc;

    // TODO dynamically reallocate if size larger than current size...
    char input[BUFFER_SIZE];

    // We read all input as per the length in the header, which should include the delimiter and the
    // null
    if (readFile(client_request_channel, input, length) == length) {

        cJSON *root = jsonParse(input);
        cJSON *id = jsonGetObjectItem(root, "id");
        cJSON *method = jsonGetObjectItem(root, "method");

        if (method != NULL) {

            if (id != NULL) {
                // A request
                log_trace("client -> server : '%s' request (%d)", method->valuestring, id->valueint);
                int result = jsonSend(root, server_request_channel);
                if (result == EOF)
                    rc = RC_ERROR_SENDING_TO_SERVER;
                if (strcmp(method->valuestring, "exit") == 0)
                    rc = RC_EXIT;
            } else {
                // A notification
                log_trace("client -> server : '%s' notification", method->valuestring);
                int result = jsonSend(root, server_request_channel);
                if (result == EOF)
                    rc = RC_ERROR_SENDING_TO_SERVER;
            }
            jsonDelete(root);

        } else {
            log_warn("Received invalid LSP request");
        }

    } else {
        log_error("Broken input channel from client");
        rc = RC_BROKEN_INPUT_CHANNEL_FROM_CLIENT;
    }
    return rc;
}
