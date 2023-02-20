include ../Makefile.boilerplate

$(TEST): dummy_server
	../../src/clef --lsp=./dummy_server < input > output
	$(VERIFY)

CFLAGS = -g -O0
LDLIBS = -lcjson
dummy_server: dummy_server.c
