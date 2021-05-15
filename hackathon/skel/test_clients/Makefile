CC=gcc
CFLAGS = -fPIC -Wall
LDFLAGS = -L.
LDLIBS = -llogmemcache
CLIENTS= client1 client2 client3 client4 client5 client6

.PHONY: build
build: $(CLIENTS)

.PHONY: liblogmemcache.so
liblogmemcache.so: ../liblogmemcache.so
	cp ../liblogmemcache.so .

client1: client1.o liblogmemcache.so

client1.o: client1.c

client2: client2.o liblogmemcache.so

client2.o: client2.c

client3: client3.o liblogmemcache.so

client3.o: client3.c

client4: client4.o liblogmemcache.so

client4.o: client4.c

client5: client5.o liblogmemcache.so

client5.o: client5.c

client6: client6.o liblogmemcache.so

client6.o: client6.c

.PHONY: clean
clean:
	rm -f client*.o $(CLIENTS) liblogmemcache.so
