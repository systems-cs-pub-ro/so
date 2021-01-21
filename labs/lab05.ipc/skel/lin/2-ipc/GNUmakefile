CC=gcc
CFLAGS=-g -Wall -W -Wno-unused-variable -Wno-unused-parameter
CPPFLAGS=-D_DEBUG -I../utils
LDLIBS=-lrt

UNIX_IPC_OBJ= unix_queue.o unix_shm.o unix_sem.o

BINARIES = client server

.PHONY: all clean

all: $(BINARIES)

client: client.o hash.o hashtable.o $(UNIX_IPC_OBJ)

server: server.o hash.o hashtable.o $(UNIX_IPC_OBJ)


unix_shm.o: unix_shm.c

unix_queue.o: unix_queue.c

unix_sem.o: unix_sem.c


hashtable.o: hashtable.c

hash.o: hash.c



.PHONY: clean

clean:
	rm -f $(BINARIES) *.o
	../utils/clean.sh
