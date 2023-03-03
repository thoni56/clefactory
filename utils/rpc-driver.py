#!/usr/bin/env python

# Usage:
#     rpc-driver.py <file> [ <startup-delay> [ <message-delay> ] ]
#
# Read lines from the file argv[1] and prepend them with a JSON-RPC header
# If there is a second argument that is the delay before starting, in seconds
# and if there is a third that is the delay between messages

import json
import sys
import time

# Open the input file
with open(sys.argv[1], 'r') as input_file:
    # Iterate over each line in the input file
    if len(sys.argv) > 2:
        time.sleep(int(sys.argv[2]))
    for line in input_file:
        if len(sys.argv) > 3:
            time.sleep(int(sys.argv[3]))
        else :
            time.sleep(0.01)
        # Strip whitespace from the line
        line = line.strip()

        # Calculate the length of the JSON string in bytes
        content_length = len(line)+4

        # Send header
        sys.stdout.write('Content-Length: %d\r\n' % content_length)
        sys.stdout.write('Content-Type: application/vscode-jsonrpc;charset=utf-8\r\n')
        sys.stdout.write('\r\n')

        # Send payload with delimiter
        sys.stdout.write('%s\r\n' % line)
        sys.stdout.write('\r\n')
        sys.stdout.flush()
