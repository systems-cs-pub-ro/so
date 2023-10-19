CPPFLAGS = -I../utils
CFLAGS = -Wall -Wextra -Wno-unused-function -g -fstack-protector -fno-PIC
LDFLAGS = -no-pie

.PHONY: all clean

all: socket_ssp

socket_ssp: socket_ssp.o

clean:
	-rm -f socket_ssp.o socket_ssp
	-rm -f *~
