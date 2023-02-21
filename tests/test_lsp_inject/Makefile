include ../Makefile.boilerplate


$(TEST): dummy_server
	../../src/clef --lsp=./dummy_server --log=log < input 2> error
	cat log error | $(NORMALIZE_TRACE) > output
	$(VERIFY)

CFLAGS = -g -O0
LDLIBS = -lcjson
dummy_server: dummy_server.c
