CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -g -Wno-unused-variable -Wno-unused-but-set-variable -fno-stack-protector

.PHONY: all clean

all: overflow

overflow: overflow.o

overflow.o: overflow.c ../utils/utils.h

clean:
	-rm -f *.o *~ overflow
