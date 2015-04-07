#CPPFLAGS = -DDEBUG -DLOG_LEVEL=LOG_DEBUG
CFLAGS = -Wall -g -Wno-unused

.PHONY: all clean

all: run_test.o vmsim_test_lin.o common_lin.o vmsim_test.o

run_test.o: run_test.c run_test.h vmsim.h util.h debug.h common.h

vmsim_test_lin.o: vmsim_test_lin.c run_test.h

common_lin.o: common_lin.c common.h

vmsim_test.o: vmsim_test.c vmsim_test.h

clean:
	-rm -f *~ *.o
