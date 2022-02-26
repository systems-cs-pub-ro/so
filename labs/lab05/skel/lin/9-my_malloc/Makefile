CC = gcc
CPPFLAGS = -Wall -Wextra -g -I../utils -fPIC

.PHONY: all clean

all: test

test: test.o libmy_malloc.so
	$(CC) $(CFLAGS) -o $@ $< -lmy_malloc -L.

libmy_malloc.so: my_malloc.o
	$(CC) $(CFLAGS) -shared -fPIC $< -o $@

my_malloc.o: my_malloc.c my_malloc.h

test.o: test.c my_malloc.h

clean:
	- rm -fr *.o libmy_malloc.so test
