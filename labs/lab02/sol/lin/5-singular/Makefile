CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: singular

singular: singular.o

singular.o: singular.c ../utils/utils.h

clean:
	-rm -f *.o *~ singular *.txt
