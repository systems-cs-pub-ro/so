CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: lseek

lseek: lseek.o

lseek.o: lseek.c ../utils/utils.h

clean:
	-rm -f *.o *~ lseek *.txt
