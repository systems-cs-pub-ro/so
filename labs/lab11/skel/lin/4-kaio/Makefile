CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-variable -g
LDLIBS = -laio

.PHONY: all clean

all: kaio

kaio: kaio.o

kaio.o: kaio.c ../utils/utils.h

clean:
	-rm -f *.o *~ kaio *.txt
