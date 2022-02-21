CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: mcat

mcat: mcat.o xfile.o

mcat.o: mcat.c ../utils/utils.h

xfile.o: xfile.c xfile.h

clean:
	-rm -f *.o *~ mcat
