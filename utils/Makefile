# Makefile for all utility programs

CFLAGS = -g -O0

# In case some program needs cJSON:
ifeq ($(OS),Darwin)
LDLIBS += -L$$(brew --prefix)/lib -lcjson
else
LDLIBS += -lcjson
endif


all: dumping_server canned_server

dumping_server: dumping_server.c

canned_server: canned_server.c
