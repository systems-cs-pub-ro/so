CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g
LDLIBS = -lpthread

.PHONY: all clean

all: prodcons

prodcons: prodcons.o

prodcons.o: prodcons.c ../utils/utils.h

clean:
	-rm -f *.o *~ prodcons
