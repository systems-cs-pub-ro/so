/**
 * SO, 2017 - Lab #07, Profiling & Debugging
 * Task #6, Linux
 *
 * Flowers reloaded
 */

#include <stdio.h>
#include <stdlib.h>

const char *flowers[] = {
	"rose", "tulip", "daisy"
	"petunia", "orchid", "lily"
};

int main(void)
{
	int i;
	int choice;

	for (i = 0; i < 25; i++) {
		choice = rand() % 6;
		printf("%s\n", flowers[choice]);
	}

	return 0;
}
