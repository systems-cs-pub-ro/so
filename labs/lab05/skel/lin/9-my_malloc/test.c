/**
  * SO, 2016
  * Lab #5
  *
  * Task #10, lin
  *
  * Test for custom malloc implementation
  */
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "my_malloc.h"

#define	 SIZE	1024

int main(void)
{
	char *p = NULL, *q = NULL;

	/* test my_malloc */

	p = my_malloc(SIZE);
	DIE(p == NULL, "my_malloc failed");
	sprintf(p, "My allocated string");

	q = my_malloc(SIZE);
	DIE(q == NULL, "my_malloc failed");
	sprintf(q, "Another alocated string");

	printf("p=%s\n", p);
	printf("q=%s\n", q);


	return 0;
}
