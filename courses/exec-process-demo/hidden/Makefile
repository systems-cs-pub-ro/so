CFLAGS = -Wall -g -fno-PIC -fno-stack-protector
LDFLAGS = -no-pie

.PHONY: all clean

all: hidden

hidden: hidden.o reader.o

hidden.o: hidden.c reader.h

reader.o: reader.c

clean:
	-rm -f hidden.o reader.o hidden
	-rm -f *~
