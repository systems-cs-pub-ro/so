#
# SO, 2016 - Lab #1, Introduction
# Task #2, Linux
#
# Multiple source compiling
# Makefile

CC = gcc
CFLAGS = -Wall -DHAVE_MATH
LDLIBS = -lm
all: ops

ops: add.o mul.o ops.o

add.o: add.c
mul.o: mul.c
ops.o: ops.c

clean:
	rm -rf *.o ops
