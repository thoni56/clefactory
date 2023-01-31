#######################################################################
#
# This makefile supports the following use cases
#
# 2: ci - build and run all tests with coverage and create
#    coverage report:
#
#	make ci
#
# 3: all (or no target) - build and run unittests & build the c-xref
# exe:
#
#	make
#
# 4: build and run unittests with coverage and produce gcov files for
#    cov-mode in Emacs:
#
#	make unit
#
# 5: build with coverage, run all "quick" tests and produce
# gcov:
#
#	make devel
#
# 6: build with coverage, run one test and produce gcov files:
#
#   make
#   make clean-coverage
#   <run test>
#	make coverage-report
#
# 6: run another test and produce aggregated gcov files:
#
#	<run another test>
#	make coverage-report
#
# 7: produce coverage report for current collected coverage:
#
#	make coverage-report
#
# 8: watch sources to build and run all tests:
#
#	make watch
#
# 9: watch sources to build and run unittests (use as compile command in Emacs!):
#
#	make watch-for-unittests
#
# 10: watch unitests completion and run all systemtests (use in a separate
#     terminal):
#
#	make watch-for-systemtests
#
#########################################################################
# Always export following variables to environment of sub-makes (to use you need "-e")

# Build with coverage for all cases except "prod"
export COVERAGE = --coverage
export CC = cc

# Other variables

MAKEFLAGS += --no-print-directory

WARNINGS = -Wall -Wno-char-subscripts -Werror -Wimplicit-fallthrough

CFLAGS = -g -Og $(INCLUDES) -fPIC -MMD -funsigned-char $(WARNINGS) $(COVERAGE) $(EXTRA_CFLAGS)

LDFLAGS = $(COVERAGE)

ROOTDIR=..

# On Darwin we can use gcc-11 if available
OS := $(shell uname)
ifeq ($(OS),Darwin)
#	ifneq (, $(shell command -v gcc-11 2> /dev/null))
#		CC=gcc-11
#	endif
#	ifeq ($(shell arch),arm64)
#		# And for Homebrew on M1
#		CC=cc
#	endif
# But we'd rather go with Apple Clang and generate fat binaries
CFLAGS += -arch arm64 -arch x86_64
LDFLAGS += -arch arm64 -arch x86_64
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
	for tc in ../tests/*/.coverage ; do $(GCOV_TOOL) merge $$tc $(OBJDIR) -o $(OBJDIR) ; done

chcheck:
	for m in `echo $(MODULES) | sed -e 's/[a-z]*_parser\.tab//g'` ; do \
		chcheck $$m ; \
	done

#########################################################################
include sources.mk

GIT_HASH = $(shell git describe --abbrev=5 --always --tags --dirty)
$(OBJDIR)/options.o: CFLAGS += -DGIT_HASH=\"$(GIT_HASH)\"

PHONY += build
build: crefactory

PHONY += crefactory
crefactory: $(OBJS)
	$(CC) $(LDFLAGS) -o crefactory $(OBJS) $(LIBS)


#########################################################################
test:
	@echo "Only running quick tests for now - 'cd ../test; make all' to run all"
	$(MAKE) -e -C ../tests quick

unit: unittests


#########################################################################
clean:
	#-$(MAKE) -C ../tests clean
	-rm -rf $(OBJDIR) crefactory *.gcov


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
	@cloc --quiet *.[chy] *.t? | grep SUM: | awk '{print "Lines:", $$5}'
endif

#########################################################################
watch:
	watchexec -i \*.tab.\[ch\] -e c,h,y,mock -- execnotify -t Development make -j devel

watch-for-unittests:
	watchexec -i \*.tab.\[ch\] -e c,h,y,mock -- execnotify -t Unittests make -j unit

watch-for-systemtests:
	watchexec -i \*.tab.\[ch\] -e .done -- execnotify -t Systemtests make -j system-tests

system-tests:
	$(MAKE) -e build
	$(MAKE) -e test
	$(MAKE) coverage-report

#########################################################################
#
vpath %.dll.a /usr/local:/usr/local/lib
vpath %.so /usr/lib:/usr/local/lib
vpath %.dylib /usr/lib:/usr/local/lib


# Find all *_tests.c automatically
UNITTESTS = $(patsubst %.c,%.$(EXT),$(wildcard *_tests.c))

OS := $(shell uname)
ifeq ($(OS),Darwin)
	EXT=dylib
else
	EXT=so
endif

# If you have a make >3.something (MacOS don't!) you can use vpath to locate Cgreen library
# as a dependency which will automatically be linked
# CGREEN_LIB = -lcgreen
# else you have to add explicit linkage and/or include directives
CGREEN_INCLUDE = -I/usr/local/include
CGREEN_LIB = -L/usr/local/lib -lcgreen

ifeq ($(OS),Darwin)
	ifeq ($(shell arch),arm64)
		# And for Homebrew on M1
		CGREEN_LIB = -L/opt/homebrew/lib -lcgreen
		DYLD_PATH = DYLD_LIBRARY_PATH=/opt/homebrew/lib
	endif
endif

PHONY += unittests
ifeq ($(shell command -v cgreen-runner 2> /dev/null),)
unittests: parsers
	@echo Cgreen not available, cannot run unittests
else
unittests: $(UNITTESTS)
	$(MAKE) -e clean-coverage
	$(DYLD_PATH) cgreen-runner -q $(CGREEN_OUTPUT) --suite unittests $(UNITTESTS)
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
	$(CC) $(LDFLAGS) -shared -o $$@ $$^ $$(COVERAGE) $$(CGREEN_LIB) -lz
endef

# Autogenerate unittest targets and recipe for listed modules
$(eval $(call UNITTEST,main,))
$(eval $(call UNITTEST,dispatcher,))

.PHONY : $(PHONY)