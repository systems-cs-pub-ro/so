CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -g

.PHONY: all clean

all: adr_space

adr_space: adr_space.o

adr_space.o: adr_space.c ../utils/utils.h

clean:
	-rm -f *.o *~ adr_space
