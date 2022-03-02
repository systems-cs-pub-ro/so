/**
 * SO
 * Lab #3
 *
 * Task #1, Linux
 *
 * Use of "system" library call and use of "strace"
 */
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int main(int argc, char **argv)
{
	int ret;

	if (argc != 2) {
		printf("Usage: ./system cmd\n");
		exit(1);
	}

	ret = system(argv[1]);
	DIE(ret < 0, "system");

	return 0;
}
