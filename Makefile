CC = gcc
CFLAGS = -Wall -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong -O3 -fpic -DENABLE_THREADS -D_REENTRANT -DENABLE_SSL
LIBS = -L../src/ -lircclient -lpthread -lssl -lcrypto  -lnsl
INCLUDES=-I/usr/include/libircclient

EXECUTABLES=albion

all:	$(EXECUTABLES)

albion:	albion.o
	$(CC) -o albion albion.o $(LIBS)

clean:
	-rm -f $(EXECUTABLES) *.o *.exe

distclean: clean
	-rm -f Makefile *.log

.c.o:
	@echo "Compiling  $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
