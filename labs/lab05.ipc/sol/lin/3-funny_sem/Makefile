CC = gcc
CPPFLAGS = -I../utils/
CFLAGS = -Wall -g
LDLIBS = -lrt

all: server client

server: server.c
client: client.c

clean:
	rm -f *.o server client
	../utils/clean.sh
