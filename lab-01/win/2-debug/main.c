/**
 * SO, 2010 - Lab #1, Introduction
 * Task #2, Windows
 * 
 * Multiple source files compiling
 */

#include <stdio.h>

#include "add.h"

#ifdef DEBUG__
#define Dprintf(format, ...)                                    \
        fprintf (stderr, "[%s]:%s:%d: " format, __FILE__,       \
                        __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define Dprintf(format, ...)
#endif  /* DEBUG__ */

#define A_NUMBER	2
#define B_NUMBER	(A_NUMBER + 8)

int main(void)
{
	int res;
	Dprintf("Before add function call\n");

	res = add(A_NUMBER, B_NUMBER);

	Dprintf("After add function call\n");
	return 0;
}
