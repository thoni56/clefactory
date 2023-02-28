include ../Makefile.boilerplate

OS := $(shell uname)


$(TEST): dumping_server
	../../utils/rpc-driver.py input | ../../src/clef --lsp=./dumping_server --log=log 2> error
	cat log error dump | $(NORMALIZE_TRACE) > output
	$(VERIFY)

CFLAGS = -g -O0

ifeq ($(OS),Darwin)
LDLIBS += -L$$(brew --prefix)/lib -lcjson
else
LDLIBS += -lcjson
endif

dumping_server: dumping_server.c
