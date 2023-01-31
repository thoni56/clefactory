# 1. Use libclang

Date: 2023-01-31

## Status

Accepted

## Context

There are many ways of getting source information. The complexity of `c-xrefactory` has been found to be prohibitive and the continuing progress of the `Clang` project is inspiring.

## Decision

Use `libclang` as the backend for parsing and decorating an AST.

## Consequences

The whole sub-problem of creating a C parser and using that to get source information is solved as long as the needed information is available in the Clang AST.
