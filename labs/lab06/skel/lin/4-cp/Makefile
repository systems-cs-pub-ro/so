CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: mycp

mycp: mycp.o

mycp.o: mycp.c ../utils/utils.h


clean:
	-rm -f *.o *~ mycp
