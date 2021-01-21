CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g -fno-stack-protector
LDLIBS = -lpthread

.PHONY: all clean

all: th_stack

th_stack: th_stack.o

th_stack.o: th_stack.c ../utils/utils.h

clean:
	-rm -f *.o *~ th_stack
