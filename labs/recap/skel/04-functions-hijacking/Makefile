CC=gcc

LIB_NAME=libnative.so

all: nativelib test

nativelib: native.o
	$(CC) -Wall -fPIC -ldl -shared -o $(LIB_NAME) native.o

native.o: native.c

test: test.o
	gcc -lpthread test.o -o test

test.o: test.c

clean:
	rm -f $(LIB_NAME) *~ *.o test
