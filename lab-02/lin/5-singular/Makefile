CC = gcc
CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -g

LOCK_FILE=/tmp/my_lock_file

.PHONY: all clean

all: singular

singular: singular.o

singular.o: singular.c ../utils/utils.h

clean:
	-rm -f *.o *~ singular *.txt $(LOCK_FILE)

