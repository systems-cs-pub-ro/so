CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-parameter -g
LDLIBS = -lpthread

.PHONY: all clean

all: spin mutex

spin: malloc.c critical.c ../utils/utils.h
	$(CC) malloc.c critical.c $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -D USE_SPINLOCK -o spin

mutex: malloc.c critical.c ../utils/utils.h
	$(CC) malloc.c critical.c $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o mutex

clean:
	-rm -f *.o *~ spin mutex

