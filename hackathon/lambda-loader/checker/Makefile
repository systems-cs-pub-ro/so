SRCDIR?=$(abspath ../skel)
CC=gcc
CFLAGS=-fPIC -Wall -Werror
LDLIBS=$(SRCDIR)/libipc.so
CLIENT_LIBS=libbasic.so libadvanced.so libspecial.so

.PHONY: build
build: client $(CLIENT_LIBS)

client: client.o $(LDLIBS)

client.o: client.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(LDLIBS):
	@$(MAKE) -C $(SRCDIR) -f Makefile $(foreach LIB,$(LDLIBS),$(notdir $(LIB)))

lib%.so: %.o
	$(CC) $(LDFLAGS) -shared -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf *.o client *.so output ref/big ref/test9
