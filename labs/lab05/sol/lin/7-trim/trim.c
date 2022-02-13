/**
 * SO
 * Lab #5, Memory Management
 *
 * Task #7, Linux
 *
 * mcheck usage
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char first_name[] = "  Harry";
char last_name[]  = "    Potter";

static char *trim(char *s)
{
	/* We fix `trim` by using an auxiliary variable aux to save p.
	 * `free` is called on `aux` to free the exact allocated memory.
	 */
	char *aux;
	char *p = malloc(strlen(s) + 1);

	DIE(!p, "malloc");

	aux = p;
	strcpy(p, s);

	while (*p == ' ')
		p++;

	strcpy(s, p);
	free(aux);

	return s;
}

int main(void)
{
	printf("%s %s is learning SO!\n", trim(first_name),
			trim(last_name));
	return 0;
}
