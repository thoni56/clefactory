## Constraints

### JSON-RPC

Messages between server and client and `clef` is performed using JSON-RPC message format.

A payload is the preceeded with an RPC header

```
Content-Length: 58
Content-Type: application/vscode-jsonrpc;charset=utf-8

```

NOTE: line-separators are CRLF
NOTE: a single empty line separates the header from the payload
NOTE: the given length must include all character up to the start of the next header

### LSP - Language Server Protocol

The LSP defines a set of message, requests and responses, in JSON
format, that are exchanged between the client and the server.

Each such message is terminated by two consequtive CRLF's.

NOTE: The terminating CRLF's are included in the length stated in the header.

### TBD
