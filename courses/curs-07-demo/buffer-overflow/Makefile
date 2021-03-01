CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-function -g

.PHONY: all clean

all: buffer-overflow buffer-overflow-32 buffer-overflow-pie buffer-overflow-ssp buffer-overflow-asan

buffer-overflow: buffer-overflow.o
	$(CC) -no-pie -o $@ $^

buffer-overflow.o: buffer-overflow.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fno-stack-protector -fno-PIC -c -o $@ $<

buffer-overflow-32: buffer-overflow-32.o
	$(CC) -no-pie -m32 -o $@ $^

buffer-overflow-32.o: buffer-overflow.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fno-stack-protector -fno-PIC -m32 -c -o $@ $<

buffer-overflow-pie: buffer-overflow-pie.o
	$(CC) -pie -o $@ $^

buffer-overflow-pie.o: buffer-overflow.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fno-stack-protector -fPIC -c -o $@ $<

buffer-overflow-ssp: buffer-overflow-ssp.o
	$(CC) -no-pie -o $@ $^

buffer-overflow-ssp.o: buffer-overflow.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fstack-protector -fno-PIC -c -o $@ $<

buffer-overflow-asan: buffer-overflow-asan.o
	$(CC) -no-pie -fsanitize=address -o $@ $^

buffer-overflow-asan.o: buffer-overflow.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=address -fno-PIC -c -o $@ $<

clean:
	-rm -f buffer-overflow buffer-overflow.o
	-rm -f buffer-overflow-32 buffer-overflow-32.o
	-rm -f buffer-overflow-pie buffer-overflow-pie.o
	-rm -f buffer-overflow-ssp buffer-overflow-ssp.o
	-rm -f *~
