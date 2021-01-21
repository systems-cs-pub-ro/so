CPPFLAGS = -I../utils/
CFLAGS = -Wall -g

.PHONY: all clean

all: hello_static hello_dynamic do_chroot

hello_static: hello.o
	gcc -static -o $@ $^

hello_dynamic: hello.o
	gcc -o $@ $^

clean:
	-rm -f *.o *~
	-rm -f hello_static hello_dynamic do_chroot
