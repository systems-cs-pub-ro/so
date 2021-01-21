CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g
LDLIBS = -lpthread

.PHONY: all clean

all: processes threads

processes: processes.o

processes.o: processes.c ../utils/utils.h

threads: threads.o

threads.o: threads.c ../utils/utils.h

clean:
	-rm -f *.o *~ processes threads
