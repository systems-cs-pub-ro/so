CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-parameter -g
LDLIBS = -lpthread

.PHONY: all clean

all: spin mutex

spin: spin.o

spin.o: spin.c ../utils/utils.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -D USE_SPINLOCK -c -o spin.o spin.c

mutex: mutex.o

mutex.o: spin.c ../utils/utils.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -c -o mutex.o spin.c

clean:
	-rm -f *.o *~ spin mutex
