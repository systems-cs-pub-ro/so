CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: epoll

epoll: epoll.o

epoll.o: epoll.c ../utils/utils.h

clean:
	-rm -f *.o *~ epoll *.txt
