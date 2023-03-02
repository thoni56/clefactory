# 3. Clangd's LSP implementation as the canonical

Date: 2023-03-01

## Status

Accepted

## Context

The LSP protocol's definition of how the JSON-RPC should be used is
ambiguous, and it seems that `clangd` does not fully comply to the
commonly accepted way, e.g. if the delimiting empty line ("\r\n")
after a LSP request should be counted in the `Content-Length` we need
to decide what to do about that.

## Decision

Use the `clangd` implementation as the canonical. This means that

- a request to the server should calculate length of the payload to include any delimiting "\r\n", or, as an alternative, not even be sent since `clangd` only reads that many characters before expecting a new header
- a response from `clangd` will not include any delimiting "\r\n\r\n"
  but instead have a `Content-Length` set to only include the actual
  LSP response

## Consequences

This will ensure that `clef` will be possible to inject between any
client and `clangd` if the client works with `clangd`.

Possibly this will become a problem if we want to inject `clef` using
a client build for another LSP server. But it is unlikely that any
clients exist (for C) that are not compatible with `clangd`. If that
becomes a problem, the RCP communication have to be generalized.
