CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: trouble

trouble: trouble.o

trouble.o: trouble.c ../utils/utils.h

clean:
	rm -f *.o *~ trouble *.txt
