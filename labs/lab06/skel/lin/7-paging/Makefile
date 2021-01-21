CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: paging

paging: paging.o

paging.o: paging.c ../utils/utils.h

clean:
	-rm -f *.o *~ paging
