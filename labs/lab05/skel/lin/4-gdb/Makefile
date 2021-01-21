CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: fault

fault: fault.o

fault.o: fault.c ../utils/utils.h

clean:
	-rm -f *.o *~ fault
