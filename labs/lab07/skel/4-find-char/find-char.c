/**
 * SO, 2017 - Lab #07, Profiling
 * Task #4, Linux
 *
 * Operations on strings
 */
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>



/**
 * Read the content of a file into a string
 */
char *readstring(const char *fname)
{
	int fd;
	off_t size;
	ssize_t readb;
	char *s, *ret;

	fd = open(fname, O_RDONLY);
	if (-1 == fd)
		return NULL;

	size = lseek(fd, 0, SEEK_END);
	if (((off_t)-1) == size) {
		close(fd);
		return NULL;
	}
	if (((off_t)-1) == lseek(fd, 0, SEEK_SET)) {
		close(fd);
		return NULL;
	}
	ret = s = malloc(size+1);
	if (ret == NULL)
		return NULL;
	while (size) {
		readb = read(fd, s, size);
		if (-1 == readb) {
			if (errno == EAGAIN)
				continue;
			else {
				close(fd);
				free(ret);
				return NULL;
			}
		}
		s += readb;
		size -= readb;
	}
	close(fd);
	return ret;
}

/**
 * Searches the first occurence of `what` inside `where`
 * Returns the index or -1 in case of string not found
 */
int find_first_pos(const char *where, const char what)
{
	int i = 0;

	for (i = 0; i < strlen(where); i++)
		if (where[i] == what)
			return i;
	return -1;
}

void find_all_pos(const char *where, const char what)
{
	const char *curr;
	int pos;

	curr = where;
	while (what) {
		pos = find_first_pos(curr, what);
		if (-1 == pos)
			return;
		curr += pos + 1;
	}
}

int main(void)
{
	char *s;

	s = readstring("data.in");
	if (s == NULL) {
		perror("Could not read data\n");
		return -1;
	}
	find_all_pos(s, 'a');
	return 0;
}
