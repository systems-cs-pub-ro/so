CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: tiny

tiny: tiny.o

tiny.o: tiny.c ../utils/utils.h

clean:
	-rm -f *.o *~ tiny
