CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: poll

poll: poll.o

poll.o: poll.c ../utils/utils.h

clean:
	-rm -f *.o *~ poll *.txt
