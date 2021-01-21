CC = gcc
CFLAGS = -Wall -I../utils
HEADERS = ../utils/utils.h

all: server client

server: server.o

server.o: server.c $(HEADERS)

client: client.o

client.o: client.c $(HEADERS)

clean:
	rm -f *.o *~ server client myfifo