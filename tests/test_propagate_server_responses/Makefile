include ../Makefile.boilerplate

OS := $(shell uname)


$(TEST): canned_server
	../../src/clef --lsp=./canned_server --log=log 2> error > output.tmp
	cat output.tmp error | $(NORMALIZE_TRACE) > output
	$(VERIFY)

CFLAGS = -g -O0

ifeq ($(OS),Darwin)
LDLIBS += -L$$(brew --prefix)/lib -lcjson
else
LDLIBS += -lcjson
endif

canned_server: canned_server.c
