/**
 * SO, 2016
 * Lab #1, Introduction
 *
 * Task #2, Linux
 *
 * Building libraries
 */

#include "hexdump.h"
#include <stdlib.h>
#include <time.h>

void generate_frame(struct xframe *frame)
{
	int i;

	srand(time(NULL));
	frame->len = random() % 32;

	for (i = 0; i < frame->len; i++)
		frame->data[i] = random() % 255;
}
