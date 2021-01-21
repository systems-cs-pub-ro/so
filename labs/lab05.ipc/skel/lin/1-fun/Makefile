CC = gcc
CPPFLAGS = -I../utils/
CFLAGS = -Wall -g
LDLIBS = -lrt -lpthread

all: shm sem mq

shm: shm.c
sem: sem.c
mq: mq.c

clean:
	rm -f *.o shm sem mq
	../utils/clean.sh
