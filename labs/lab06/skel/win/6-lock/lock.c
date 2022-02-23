/**
 * SO
 * Lab #6, Virtual Memory
 *
 * Task #6, Windows
 *
 * Locking memory in order to avoid swaping
 */

/* Do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <windows.h>

#include "utils.h"

#define SIZE 128

static int pageSize = 0x1000;
char msg[] = "Very important real-time data";

/* Lock memory interval [addr, addr + size - 1] */
static void lock_memory(char *addr, DWORD size)
{
	DWORD rc;
	DWORD page_offset = (DWORD)addr % pageSize;

	/* TODO - Align addr to page offset and adjust size */

	/* TODO - Lock memory */
}

/* Unlock memory interval [addr, addr + size - 1] */
static void unlock_memory(char *addr, DWORD size)
{
	int rc;
	DWORD page_offset = (DWORD)addr % pageSize;

	/* TODO - Align addr to page offset and adjust size */

	/* TODO - Unlock memory */
}

int main(void)
{
	char data[SIZE];

	/* TODO - Lock data */

	/* TODO - Copy msg into data */

	/* Printing data to see it's there */
	printf("data=%s\n", data);

	/* TODO - Unlock data */

	return 0;
}
