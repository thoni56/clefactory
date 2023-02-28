#!/usr/bin/env python

import json
import sys

# Open the input file
with open(sys.argv[1], 'r') as input_file:
    # Iterate over each line in the input file
    for line in input_file:
        # Strip whitespace from the line
        line = line.strip()
        # Calculate the length of the JSON string in bytes
        content_length = len(line.encode('utf-8'))
        # Create the transport message as a string
        transport_message = f'Content-Length: {content_length}\r\nContent-type: application/vscode-jsonrpc;charset=utf-8\r\n\r\n{line}\r\n\r\n'
        # Write the transport message to stdout
        sys.stdout.write(transport_message)
