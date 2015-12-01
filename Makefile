EXECUTABLE=albion
CC = gcc
LIBS = -lircclient -lpthread -lssl -lcrypto  -lnsl -lcurl -lxml2
INCLUDES= -Iinclude -I/usr/include/libircclient -I/usr/include/libxml2 -I/usr/include/curl
CFLAGS = -g -Wall -march=x86-64 -mtune=generic -O2 -pipe -fstack-protector-strong -O3 -fpic -DENABLE_THREADS -D_REENTRANT -DENABLE_SSL $(INCLUDES)

HEADERS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)


$(EXECUTABLE): $(patsubst %.c, %.o, $(SRC))
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
clean:
	rm -f src/*.o  $(EXECUTABLE)
