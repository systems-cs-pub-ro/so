/**
 * SO
 * Lab #2, Simple I/O operations
 *
 * Task #5, Linux
 *
 * Locking a file
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h> /* flock */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> /* errno */

#include "utils.h"

#define LOCK_FILE "/tmp/my_lock_file"

static int fdlock = -1;

static void do_stuff(void)
{
	sleep(10);
}

static void check_lock(void)
{
	int rc;

	/**
	 * TODO - Open file
	 * Another solution that would limit the number of system calls
	 * would be using the flags O_EXCL and O_CREAT when opening the file
	 */
	fdlock = open(LOCK_FILE, O_CREAT | O_RDWR, 0644);
	DIE(fdlock < 0, "open fdlock");

	/**
	 * TODO - Lock the file using flock
	 * flock must not block in any case !
	 */
	rc = flock(fdlock, LOCK_EX | LOCK_NB);
	if (rc < 0) {
		if (errno == EWOULDBLOCK) {
			perror("Process already running");
			exit(EXIT_SUCCESS);
		} else {
		}
	} else {
		DIE(rc, "flock failed");
	}

	printf("\nGot Lock\n\n");
}

static void clean_up(void)
{
	/* TODO - Unlock file, close file and delete it */
	int rc;

	rc = flock(fdlock, LOCK_UN);
	DIE(rc < 0, "flock failed");

	rc = close(fdlock);
	DIE(rc < 0, "close");

	rc = unlink(LOCK_FILE);
	DIE(rc < 0, "unlink");
}

int main(void)
{
	check_lock();

	do_stuff();

	clean_up();

	return 0;
}
