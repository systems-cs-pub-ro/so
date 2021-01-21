CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: alloc

alloc: alloc.o

alloc.o: alloc.c ../utils/utils.h

clean:
	-rm -f *.o *~ alloc
