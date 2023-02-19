include ../Makefile.boilerplate

$(TEST): dummy_server
	../../src/clef --lsp=./dummy_server < input > output
	$(VERIFY)

LDLIBS = -lcjson
dummy_server: dummy_server.c
