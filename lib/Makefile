CC = gcc
CFLAGS = -Wall -DIN_BUILDING_LIBIRC -O2 -O3 -D_REENTRANT -DENABLE_SSL
AR=ar cr
RANLIB=ranlib
INCLUDES=-Iinclude
DESTDIR=

OBJS = libircclient.o

all: static

static:	libircclient.a

libircclient.a: libircclient.o
	$(AR) libircclient.a libircclient.o
	$(RANLIB) libircclient.a

clean:
	rm -f libircclient.a libircclient.o

distclean: clean
	-rm -f Makefile

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
