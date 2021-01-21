CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g
LDLIBS = -lpthread

.PHONY: all clean

all: once

once: once.o

once.o: once.c ../utils/utils.h

clean:
	-rm -f *.o *~ once
