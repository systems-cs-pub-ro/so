CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: mcat

mcat: mcat.o

mcat.o: mcat.c ../utils/utils.h

clean:
	-rm -f *.o *~ mcat
