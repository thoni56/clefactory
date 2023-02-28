#######################################################################
#
# This makefile supports the following targets
#
# ci - build and run all tests with coverage and create coverage report
# all (or no target) - build and run unittests & build the clef exe
# unit - build and run unittests with coverage and produce gcov files for cov-mode in Emacs
# devel - build with coverage, run all tests and produce gcov
# clean-coverage - clean coverage
# coverage-report - create coverage-report with the current data
# watch - watch sources to build and run all tests on change
# watch-for-unittests - watch sources to build and run unittests on change
# watch-for-systemtests - watch unitests completion and run all systemtests
#
#########################################################################
OS := $(shell uname)

# Build with coverage for all cases except "prod"
export COVERAGE = --coverage
export CC = cc

# Includes and libs
ifeq ($(OS),Darwin)
  LLVM = /opt/clang+llvm-15.0.6-arm64-apple-darwin21.0
else
  LLVM = /usr/lib/llvm-15
endif

ifeq ($(OS),Darwin)
INCLUDES += -I$(LLVM)/include -I$$(brew --prefix)/include
LDLIBS += -L$(LLVM)/lib -lclang -L$$(brew --prefix)/lib -lcjson
else
INCLUDES += -I$(LLVM)/include
LDLIBS += -L$(LLVM)/lib -lclang -lcjson
endif

# Other variables

MAKEFLAGS += --no-print-directory

WARNINGS = -Wall -Wno-char-subscripts -Werror -Wimplicit-fallthrough

CFLAGS = -g -O0 $(INCLUDES) -fPIC -MMD -funsigned-char $(WARNINGS) $(COVERAGE) $(EXTRA_CFLAGS)

LDFLAGS += -g $(COVERAGE)

OS := $(shell uname)
ifeq ($(OS),Darwin)
CFLAGS += -arch arm64
LDFLAGS += -arch arm64
endif

# We need to use compatible gcov (for Darwin/Brew with Homebrew gcc-11 we need to use GCC:s gcov)
ifeq ("$(CC)", "gcc-11")
	LCOV = lcov --gcov-tool gcov-11
	GCOV = gcov-11
	GCOV_TOOL = gcov-tool-11
else
	LCOV = lcov
	GCOV = gcov
	GCOV_TOOL = gcov-tool
endif

# We might not have gcov-tool available
ifneq (, $(shell command -v $(GCOV_TOOL) 2> /dev/null))
    HAVE_GCOV_TOOL=yes
else
    HAVE_GCOV_TOOL=no
endif

# If we don't then we can generate .gcov from the .info but we need:
ifneq (, $(shell command -v info2gcov 2> /dev/null))
    HAVE_INFO2GCOV=yes
else
    HAVE_INFO2GCOV=no
endif


all: unit build

#########################################################################
# For development - various ways to build
devel:
	$(MAKE) -e clean-coverage
	$(MAKE) -e devel-proper
	$(MAKE) coverage-report

devel-proper: unit build
	$(MAKE) -e test

ci:
	$(MAKE) -e -j unit
	$(MAKE) -e -j build
	$(MAKE) -e -j -C ../tests all
	-for tc in ../tests/*/.coverage ; do $(GCOV_TOOL) merge $$tc $(OBJDIR) -o $(OBJDIR) ; done

chcheck:
	for m in `echo $(MODULES) | sed -e 's/[a-z]*_parser\.tab//g'` ; do \
		chcheck $$m ; \
	done

#########################################################################
include sources.mk

GIT_HASH = $(shell git describe --abbrev=5 --always --tags --dirty)
$(OBJDIR)/options.o: CFLAGS += -DGIT_HASH=\"$(GIT_HASH)\"

PHONY += build
build: clef


clef: $(OBJS)
	$(CC) $(LDFLAGS) -o clef $(OBJS) $(LDLIBS)


#########################################################################
test:
	$(MAKE) -e -C ../tests

unit: unittests


#########################################################################
clean:
	#-$(MAKE) -C ../tests clean
	-rm -rf $(OBJDIR) clef clefactory *.gcov


########################################################################
PHONY += clean-coverage
clean-coverage:
	-rm ../coverage/total.info
	-if command -v $(LCOV) > /dev/null 2>&1 ; then \
		$(LCOV) -q -z -d .. ; \
	fi

# Create .gcov files from .gcda in $(OBJDIR)
gcov:
	-$(GCOV) $(OBJDIR)/*.o --object-directory $(OBJDIR) 2>&1 >/dev/null | grep -v "assuming not executed" | cat

coverage-report:
# We create separate coverage files for each test to be able to run them in parallel (see ../tests/Makefile)
ifeq ($(HAVE_GCOV_TOOL),yes)
# Then we can merge all .gcda for all tests into the .gcda in OBJDIR using it, which is very fast!
	-for tc in ../tests/*/.coverage ; do $(GCOV_TOOL) merge $$tc $(OBJDIR) -o $(OBJDIR) ; done
# And then create .gcov for Emacs cov-mode
	make gcov
else
# We need to collect all coverage before we can generate gcov...
endif
	@echo Doing the actual reporting in the background...
	$(MAKE) the-actual-coverage-report &

LCOV_IGNORE_COVERAGE_FOR = 'log.c' '*.mock' '/usr/*' '/Library/*' '/Applications/*'

LCOV_TEST_DIRS = `ls -d ../tests/*/.coverage | xargs printf -- '-d %s\n'`
ifeq ($(OS),Darwin)
SYSTEM_TEST_COVERAGE_FILES = `find ../tests -name coverage.info | xargs stat -f "%N %z" | grep -v " 0" | awk '{print $$1}' | xargs printf -- '-a %s\n'`
else
SYSTEM_TEST_COVERAGE_FILES = `ls ../tests/*/.coverage/coverage.info | xargs printf -- '-a %s\n'`
endif

the-actual-coverage-report:
	@mkdir -p ../coverage
ifeq ($(HAVE_GCOV_TOOL),yes)
# Then we have already merged all test .gcda into OBJDIR and can just generate the total.info from .
	$(LCOV) -d . -c -q -o ../coverage/total.info 2>&1 | grep -v "did not produce any data" | cat
else
# The run_test script in ../tests has created a coverage.info in each test directory
# So we can add each of the tests coverage.info to ../coverage/total.info using '-a ...'
	$(LCOV) $(SYSTEM_TEST_COVERAGE_FILES) -q -o ../coverage/total.info 2>&1
endif
	@echo "LCOV-ing done..."
	$(LCOV) -q --remove ../coverage/total.info $(LCOV_IGNORE_COVERAGE_FOR) -o ../coverage/total.info
	@echo "LCOV --remove done..."
ifeq ($(HAVE_GCOV_TOOL),no)
# Then we have to wait until now to generate the gcov's for Emacs cov-mode...
# And we need the info2gcov command (https://github.com/thoni56/info2gcov)
ifeq ($(HAVE_INFO2GCOV),yes)
	info2gcov -q ../coverage/total.info
else
# We have to do without the .gcov files, no Emacs coverage display sadly...
endif
endif
	genhtml -q -o ../coverage/$(COVERAGE_CASE) ../coverage/total.info
	@echo Coverage done: `grep -m 1 coverPer ../coverage/index.html | tr -dc '0-9.'`
ifneq ($(shell command -v cloc 2> /dev/null),)
	@cloc --quiet *.[ch] | grep SUM: | awk '{print "Lines:", $$5}'
endif

#########################################################################
# watchexec 1.20.6 (which my Homebrew currently has) has a bug that
# can be worked around with '--project-origin .'
ifeq ($(OS),Darwin)
  WATCHEXEC = watchexec --project-origin .
else
  WATCHEXEC = watchexec
endif

watch:
	$(WATCHEXEC) -e c,h,y,mock -- execnotify -t Development make -j devel

watch-for-unittests:
	$(WATCHEXEC) -e c,h,y,mock -- execnotify -t Unittests make -j unit

watch-for-systemtests:
	$(WATCHEXEC) -e done -- execnotify -t Systemtests make -j system-tests

system-tests:
	$(MAKE) -e build
	$(MAKE) -e test
	$(MAKE) coverage-report

#########################################################################
#
# Find all *_tests.c automatically
UNITTESTS = $(patsubst %.c,%.$(EXT),$(wildcard *_tests.c))

OS := $(shell uname)
ifeq ($(OS),Darwin)
	EXT=dylib
else
	EXT=so
endif

# If you have a make >3.something (MacOS don't unless you use brew)
# you can use vpath to locate Cgreen library as a dependency which
# will automatically be linked

# vpath %.dll.a /usr/local:/usr/local/lib
# vpath %.so /usr/lib:/usr/local/lib
# vpath %.dylib /usr/lib:/usr/local/lib

# CGREEN_LIB = -lcgreen
# else you have to add explicit linkage and/or include directives
# CGREEN_INCLUDE = -I/usr/local/include
# CGREEN_LIB = -L/usr/local/lib -lcgreen

ifeq ($(OS),Darwin)
	# Build only for M1
	CGREEN = arch -arm64 cgreen-runner
	CGREEN_LIB = -L/opt/homebrew/lib -lcgreen
	LDLIBS += -rpath $(LLVM)/lib
else
	CGREEN = cgreen-runner
	LDLIBS += -Wl,-rpath=$(LLVM)/lib
endif

PHONY += unittests
ifeq ($(shell command -v cgreen-runner 2> /dev/null),)
unittests:
	@echo Cgreen not available, cannot run unittests
else
unittests: $(UNITTESTS)
	$(MAKE) -e clean-coverage
	$(CGREEN) -q $(CGREEN_OUTPUT) --suite unittests $(UNITTESTS)
	make gcov
	filecount=`ls -1 $(OBJDIR)/*.gcda 2>/dev/null | wc -l` ; \
	if [ $$filecount -gt 0 ] ; then \
		$(LCOV) -q -t unittests -d . -c -o unittests.i 2>&1 | grep -v "did not produce any data" ; \
		$(LCOV) -q --remove unittests.i $(LCOV_IGNORE_COVERAGE_FOR) -o unittests.i ; \
		mv unittests.i unittests.info ; \
	fi
	touch .unittests.done
endif

# A test lib for a module is built from its .o and the _test.o (and some extras)
#%_tests.$(EXT): %.o %_tests.o $(EXTRA_OBJS)
#	$(CC) -shared -o $@ $(sort $(EXTRA_OBJS) $^) $(LDLIBS) -lcgreen


# Macro for unittests that link the SUT and a small number of dependent .o
# Parameters: <module>, <dependent modules ...>
define UNITTEST
$(1)_DEPENDENCIES = $(2)
$(1)_DEPS = $$(patsubst %,$(OBJDIR)/%.o,$$($(1)_DEPENDENCIES))
$(OBJDIR)/$(1)_tests.o: CFLAGS += $(CGREEN_INCLUDE)
$(1)_tests.$(EXT): $(OBJDIR)/$(1)_tests.o $(OBJDIR)/$(1).o $$($(1)_DEPS)
	$(CC) $(LDFLAGS) -shared -o $$@ $$^ $$(COVERAGE) $$(CGREEN_LIB)
endef

# Autogenerate unittest targets and recipe for listed modules
$(eval $(call UNITTEST,client_handler,log))
$(eval $(call UNITTEST,dispatcher,))
$(eval $(call UNITTEST,exports,))
$(eval $(call UNITTEST,filemanager,))
$(eval $(call UNITTEST,includes,))
$(eval $(call UNITTEST,json,log))
$(eval $(call UNITTEST,lsp,log))
$(eval $(call UNITTEST,main,log))
$(eval $(call UNITTEST,options,log))
$(eval $(call UNITTEST,parser,))
$(eval $(call UNITTEST,references,))
$(eval $(call UNITTEST,repl,))
$(eval $(call UNITTEST,server_handler,log))
$(eval $(call UNITTEST,units,))

.PHONY : $(PHONY)
