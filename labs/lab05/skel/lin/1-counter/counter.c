/**
  * SO, 2016
  * Lab #5
  *
  * Task #1, lin
  *
  * Process memory zones
  */
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * TODO - inc() function must behave like a counter
 *      The functin returns the next value in order and
 *      it does not receive paramters
 *
 *	You are not allowed to use global variables
 */
int inc(void)
{

}


int main(void)
{
	int i;

	for (i = 0; i < 10; i++)
		printf("%d\n", inc());

	return 0;
}

