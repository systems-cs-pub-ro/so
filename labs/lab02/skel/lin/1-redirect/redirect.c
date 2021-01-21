/**
 * SO, 2017
 * Lab #2, Operatii I/O simple
 *
 * Task #1, Linux
 *
 * Redirection example with "dup" and "dup2"
 * and use of "lsof"
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"

static void wait_for_input(const char *msg)
{
	char buf[32];

	printf(" * %s\n", msg);
	printf(" -- Press ENTER to continue ..."); fflush(stdout);
	fgets(buf, 32, stdin);
}


int main(void)
{
	int fd1, fd2, rc;

	wait_for_input("beginning");

	/* open files */
	fd1 = open("tmp1.txt", O_CREAT | O_RDWR, 0644);
	DIE(fd1 < 0, "open tmp1.txt");
	wait_for_input("created tmp1.txt");

	fd2 = open("Makefile", O_RDONLY);
	DIE(fd2 < 0, "open Makefile");
	wait_for_input("opened Makefile");

	/* redirect stderr to fd1 */
	rc = close(STDERR_FILENO);
	DIE(rc < 0, "close stderr");
	wait_for_input("closed stderr");

	rc = dup(fd1);
	DIE(rc < 0, "dup fd1");

	/* write something to stderr */
	write(STDERR_FILENO, "something", strlen("something"));
	wait_for_input("dup - redirected stderr to fd1. Written "
                       "something to STDERR_FILE. Inspect the associated file.");

	rc = close(fd1);
	DIE(rc < 0, "close fd1");
	wait_for_input("closed fd1");

	/* redirect stderr to fd2 */
	rc = dup2(fd2, STDERR_FILENO);
	DIE(rc < 0, "dup2 fd2");
	wait_for_input("dup2 - redirected stderr to fd2");

	rc = close(fd2);
	DIE(rc < 0, "close fd2");
	wait_for_input("closed fd2");

	return 0;
}
