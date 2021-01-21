CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: mynohup

mynohup: mynohup.o

mynohup.o: mynohup.c ../utils/utils.h

clean:
	-rm -f *.o *~ mynohup nohup.out
