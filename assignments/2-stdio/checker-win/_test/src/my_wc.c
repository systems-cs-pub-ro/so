#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#define BUFSIZE 128

/*
 * Trivial wc -c implementation
 *
 * Used in test_popen_write
 */
int main()
{
	char buf[BUFSIZE];
	int total = 0;
	int ret;

	_setmode(_fileno(stdin), _O_BINARY);

	while (!feof(stdin)) {
		ret = fread(buf, 1, BUFSIZE, stdin);
		if (ret > 0) {
			total += ret;
		}
	}

	printf("%d\n", total);

	return 0;
}
