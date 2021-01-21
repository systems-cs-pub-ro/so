CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: hack

hack: hack.o

hack.o: hack.c ../utils/utils.h

clean:
	-rm -f *.o *~ hack
