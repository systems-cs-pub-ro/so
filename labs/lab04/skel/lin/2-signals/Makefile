CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: signals

signals: signals.o

signals.o: signals.c ../utils/utils.h

clean:
	-rm -f *.o *~ signals
