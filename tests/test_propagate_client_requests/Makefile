include ../Makefile.boilerplate

OS := $(shell uname)


$(TEST): utils
	../../utils/rpc-driver.py input | ../../src/clef --lsp=../../utils/dumping_server --log=log 2> error
	cat log error dump | $(NORMALIZE_TRACE) > output
	$(VERIFY)

CFLAGS = -g -O0

utils:
	make -C ../../utils
