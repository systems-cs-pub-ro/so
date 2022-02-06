/**
  * SO, 2016
  * Lab #5
  *
  * Task #4, lin
  *
  * Use of gdb to solve "Segmentation fault" problems
  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

char *read_message()
{
	char *buf;

	buf = malloc(BUFSIZ * sizeof(char));
	DIE(!buf, "malloc");

	printf("Give input string: ");
	fgets(buf, 1024, stdin);

	return buf;
}

void upper_string(char *msg)
{
	int i = 0;

	while (msg[i] != '\0') {
		if (msg[i] >= 'a' && msg[i] <= 'z')
			msg[i] -= 32;
		i++;
	}
}

int main(void)
{
	char *message;

	message = read_message();

	upper(message);

	upper_string("Upper string is: %s\n", message);

	free(message);

	return 0;
}
