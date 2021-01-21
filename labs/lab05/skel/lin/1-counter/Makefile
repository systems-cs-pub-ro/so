CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: counter

counter: counter.o

counter.o: counter.c ../utils/utils.h

clean:
	-rm -f *.o *~ counter
