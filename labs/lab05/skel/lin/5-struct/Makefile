CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: struct

struct: struct.o

struct.o: struct.c ../utils/utils.h

clean:
	-rm -f *.o *~ struct
