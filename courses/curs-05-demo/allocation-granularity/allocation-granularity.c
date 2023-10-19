#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	size_t bytes;
	char *ptr;

	if (argc != 2) {
		fprintf(stderr, "Usage: $0 allocation-size\n\n");
		exit(EXIT_FAILURE);
	}

	sleep(5);
	bytes = strtol(argv[1], NULL, 10);
	printf("Allocating %zu bytes\n", bytes);
	ptr = malloc(bytes);

	sleep(5);
	printf("Freeing %zu bytes\n", bytes);
	free(ptr);

	sleep(5);

	return 0;
}
