CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: magic

magic: magic.o

magic.o: magic.c ../utils/utils.h

clean:
	-rm -f *.o *~ magic
