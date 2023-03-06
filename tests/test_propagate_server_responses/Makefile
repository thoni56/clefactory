include ../Makefile.boilerplate

OS := $(shell uname)


$(TEST): utils
	../../src/clef --lsp=../../utils/canned_server --log=log 2> error > output.tmp
	cat output.tmp error | $(NORMALIZE_TRACE) > output
	$(VERIFY)

utils:
	$(MAKE) -C ../../utils
