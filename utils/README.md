# Clefactory utilities

Here are the utilities that are used when developing the `clef` LSP C
refactoring extension.

It is benefitial if each test that requires these have the following
as a dependent to their top level Makefile rule

    utils:
    	$(MAKE) -C ../../utils

## Canned Server

The `canned_server` takes each line in a file as one single LSP JSON
response and returns them one by one to the client after adding an
appropriate header.

There is a short delay (100ms) between each so that the client might
get and act upon some other input before next response is sent.

## Dumping Server

The `dumping_server` will receive each request and dump them into a
file (currently `dump`).
