CC = gcc
CFLAGS = -Wall -fPIC

LDFLAGS = -L.

all:  main_dynamic

main_dynamic: main.o libhexdump_dynamic.so
	$(CC) $(CFLAGS) -o $@ $< -lhexdump_dynamic $(LDFLAGS)

libhexdump_dynamic.so: hexdump.o sample.o
	$(CC) -shared -o $@ $^

main.o: main.c
hexdump.o: hexdump.c
sample.o: sample.c

clean:
	rm -f *.o  main_dynamic libhexdump_dynamic.so
