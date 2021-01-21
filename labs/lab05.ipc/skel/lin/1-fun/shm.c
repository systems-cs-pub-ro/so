/**
  * SO, 2011
  * Lab #5
  *
  * Task #1, lin
  *
  * Using a shared memory area
  */
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "utils.h"

static int wait_for_input(const char *msg)
{
	char buf[32];

	printf("-- %s --\n", msg);
	fflush(stdout);
	fgets(buf, 32, stdin);
	if (strncasecmp(buf, "e", 1) == 0) {
		return 0;
	}
	return 1;
}


int main(void)
{
	int fd, rc;
	char *addr;
	char wr[] = "Who's the White Rabbit?";
	char rd[] = "RD";
	
	int pagesize = getpagesize();

	fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0644);
	DIE(fd < 0, "shm_open");

	rc = ftruncate(fd, pagesize);
	DIE(rc < 0, "ftruncate");

	addr = mmap(NULL, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	DIE(addr == NULL, MAP_FAILED);

	memcpy(addr, wr, strlen(wr) + 1);
	memcpy(addr + pagesize - strlen(rd) - 1 , rd, strlen(rd) + 1);

	wait_for_input("Press any key to continue...");

	rc = munmap(addr, pagesize);
	DIE(rc < 0, "munmap");
	
	rc = close(fd);
	DIE(rc < 0, "close");

	rc = shm_unlink("/my_shm");
	DIE(rc < 0, "unlink");

	return 0;
}
