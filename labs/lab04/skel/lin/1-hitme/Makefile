CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: hitme

hitme: hitme.o

hitme.o: hitme.c ../utils/utils.h

clean:
	-rm -f *.o *~ hitme
