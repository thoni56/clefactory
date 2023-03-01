#!/usr/bin/env python

import json
import sys
import time

# Open the input file
with open(sys.argv[1], 'r') as input_file:
    # Iterate over each line in the input file
    time.sleep(1)
    for line in input_file:
        # Strip whitespace from the line
        line = line.strip()

        # Calculate the length of the JSON string in bytes
        content_length = len(line)

        # Send header
        sys.stdout.write('Content-Length: %d\r\n' % content_length)
        sys.stdout.write('Content-Type: application/vscode-jsonrpc;charset=utf-8\r\n')
        sys.stdout.write('\r\n')

        # Send payload with delimiter
        sys.stdout.write('%s\r\n' % line)
        sys.stdout.write('\r\n')
        sys.stdout.flush()
