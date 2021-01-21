CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: intro

intro: intro.o

intro.o: intro.c ../utils/utils.h


clean:
	-rm -f *.o *~ intro
