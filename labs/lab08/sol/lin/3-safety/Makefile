CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-parameter -g
LDLIBS = -lpthread

.PHONY: all clean

all: spin mutex

spin: malloc.o critical_spin.o ../utils/utils.h
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) --std=c99 -DUSE_SPINLOCK -o $@

mutex: malloc.o critical.o ../utils/utils.h
	$(CC) $^ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -o $@

critical_spin.o: critical.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) --std=c99 -DUSE_SPINLOCK -o $@ $<

critical.o: critical.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	-rm -f *.o *~ spin mutex

