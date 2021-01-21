CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -g
LDLIBS = -lpthread

.PHONY: all clean

all: parallel_fgrep serial_fgrep

parallel_fgrep: parallel_fgrep.o

serial_fgrep: serial_fgrep.o

parallel_fgrep.o: parallel_fgrep.c ../utils/utils.h

serial_fgrep.o: serial_fgrep.c ../utils/utils.h

clean:
	-rm -f *.o *~ parallel_fgrep serial_fgrep
