CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: mytimer

mytimer: mytimer.o
	$(CC) $(CFLAGS) -o mytimer mytimer.o -lrt

mytimer.o: mytimer.c ../utils/utils.h

clean:
	-rm -f *.o *~ mytimer
