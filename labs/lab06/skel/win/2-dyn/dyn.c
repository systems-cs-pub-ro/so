/**
 * SO, 2014
 * Lab #6, Memoria virtuala
 *
 * Task #2, Windows
 *
 * Creating and storing functions dynamically
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE


#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <windows.h>

#include "utils.h"

static int pageSize = 0x1000;

/* Pointer to a function with one int parameter, returning int */
int (*dyncode)(int);

/* init array with code that acts like a function which:
 * (1) gets a parameter from stack
 * (2) increments that value
 * (3) returns that value
 */
unsigned char code[] = {
	0x8B, 0x44, 0x24, 0x04,  /* mov eax, [esp+4] (1) */
	0x40,			 /* inc eax          (2) */
	0xC3			 /* ret              (3) */
};

int main(void)
{
	/* static code will be copied to an area alocated with VirtualAlloc
	 * - ergo any dynamically generated code could be treated this way
	 */

	dyncode = (int (*)(int)) VirtualAlloc(NULL, pageSize,
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	DIE(dyncode == NULL, "VirtualAlloc");

	memcpy(dyncode, code, sizeof(code));

	printf("valoare intoarsa = %d\n", (*dyncode)(2));

	VirtualFree(dyncode, 0, MEM_RELEASE);

	return 0;
}
