CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: prot

prot: prot.o

prot.o: prot.c ../utils/utils.h

clean:
	-rm -f *.o *~ prot
