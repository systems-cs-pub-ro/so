CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: redirect

redirect: redirect.o

redirect.o: redirect.c ../utils/utils.h

clean:
	-rm -f *.o *~ redirect *.txt
