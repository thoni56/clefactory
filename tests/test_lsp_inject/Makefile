include ../Makefile.boilerplate


$(TEST): dummy_server dump_server
	../../utils/rpc-driver.py input | ../../src/clef --lsp=./dummy_server --log=log 2> error
	cat log error | $(NORMALIZE_TRACE) > output
	$(VERIFY)

CFLAGS = -g -O0
LDLIBS = -lcjson
dummy_server: dummy_server.c

dump_server: dump_server.c
