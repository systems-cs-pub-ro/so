CC = gcc
CPPFLAGS = -I../utils/
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: bug

bug: bug.o

bug.o: bug.c ../utils/utils.h

clean:
	-rm -f *.o *~ bug ef_bug
