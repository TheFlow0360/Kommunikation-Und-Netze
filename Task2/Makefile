# Makefile for webserver

ARCH64=''
ifeq (64,$(findstring 64,$(shell uname -m)))
	ARCH64=64
endif

CC	= gcc
CFLAGS  += -Wall -g -Iinclude -Wpedantic -std=c11 -D_XOPEN_SOURCE=600
LDFLAGS += -Llib
LDLIBS  += -lcnaiapi$(ARCH64) -lpthread

APPS = webserver

apps: $(APPS)
webserver: webserver.c request.c httphandler.c

.PHONY: clean
clean:
	; rm -rf *.o $(APPS) ; rm -rf *.dSYM
