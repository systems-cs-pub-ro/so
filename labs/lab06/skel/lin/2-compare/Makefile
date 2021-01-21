CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

.PHONY: all clean

all: write mmap

write: write.o

write.o: write.c ../utils/utils.h


mmap: mmap.o

mmap.o: mmap.c ../utils/utils.h


clean:
	-rm -f *.o *~ write mmap test_*
