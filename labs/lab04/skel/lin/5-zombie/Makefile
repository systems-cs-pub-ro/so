CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: mkzombie nozombie

mkzombie: mkzombie.o

mkzombie.o: mkzombie.c ../utils/utils.h

nozombie: nozombie.o

nozombie.o: nozombie.c ../utils/utils.h

clean:
	-rm -f *.o *~ mkzombie nozombie
