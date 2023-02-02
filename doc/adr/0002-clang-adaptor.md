# 2. Clang Adaptor

Date: 2023-02-02

## Status

Accepted

## Context

`libclang` has a complex API that we want to abstract but at least be able to mock.

## Decision

Introduce an adaptor which implements all `libclang` functions but without the initial `clang_`, so e.g. `clang_createIndex()` will be used everywhere through `createIndex()`.

## Consequences

This enables mocking and unittesting, although cumbersome as every
single `libclang` function that is needed will have to be duplicated.
