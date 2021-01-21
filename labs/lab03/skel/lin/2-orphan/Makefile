CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: orphan

orphan: orphan.o

orphan.o: orphan.c ../utils/utils.h

clean:
	-rm -f *.o *~ orphan
