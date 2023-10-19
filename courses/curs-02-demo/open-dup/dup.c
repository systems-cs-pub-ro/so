#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"

static void msg_and_wait(const char *msg)
{
	char buf[256];

	puts(msg);
	fputs("     Press ENTER to continue ...", stdout); fflush(stdout);

	while(1) {
		fgets(buf, 256, stdin);
		if (buf[strlen(buf)-1] == '\n')
			break;
	}
}

int main(void)
{

	int fd1, fd2, rc, pos;
	
	msg_and_wait("open start");

	fd1 = open("Makefile", O_RDWR);
	DIE(fd1 < 0, "open file.txt");
	msg_and_wait("open Makefile");

	fd2 = dup(fd1);
	DIE(fd2 < 0, "dup");
	msg_and_wait("fd2 = dup(fd1)");

	pos = lseek(fd1, 100, SEEK_SET);
	DIE(pos < 0, "lseek");
	msg_and_wait("lseek on fd1");
	
	rc = close(fd1);
	DIE(rc < 0, "fd1");
	msg_and_wait("close fd1");

	rc = close(fd2);
	DIE(rc < 0, "fd2");
	msg_and_wait("close fd2");

	return 0;
}
