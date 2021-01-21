/**
 * SO, 2016
 * Lab #1, Introducere
 *
 * Task #2, Windows
 *
 * Multiple source files compiling
 * add.h - addition header file
 */

#ifndef _ADD_H
#define _ADD_H

#include <stdio.h>

#ifdef DEBUG__
#define Dprintf(format, ...)                                \
	fprintf(stderr, "[%s]:%s:%d: " format, __FILE__,     \
			__FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(format, ...)
#endif  /* DEBUG__ */


int add(int a, int b);

#endif
