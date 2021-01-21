CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: askexit

askexit: askexit.o

askexit.o: askexit.c ../utils/utils.h

clean:
	-rm -f *.o *~ askexit
