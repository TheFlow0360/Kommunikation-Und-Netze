# Makefile for webserver

ARCH64=''
ifeq (64,$(findstring 64,$(shell uname -m)))
	ARCH64=64
endif

CC	= gcc *.c *.h
CFLAGS  += -Wall -g -Iinclude -Wpedantic -std=c11
LDFLAGS += -Llib
LDLIBS  += -lcnaiapi$(ARCH64) -lpthread

APPS = webserver

apps: $(APPS)


.PHONY: clean
clean:
	rm -rf *.o $(APPS)
