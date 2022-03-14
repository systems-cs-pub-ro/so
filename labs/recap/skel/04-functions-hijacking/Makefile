CC = gcc
CFLAGS = -fPIC
LDLIBS = -lpthread
LIB_NAME = libnative.so

all: $(LIB_NAME) test

$(LIB_NAME): native.o
	$(CC) -shared -o $@ $^ -ldl

clean:
	rm -f $(LIB_NAME) *~ *.o test
