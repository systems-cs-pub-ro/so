#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "ipc.h"


int create_socket()
{
	return -1;
}

int connect_socket(int fd)
{
	return -1;
}

ssize_t send_socket(int fd, const char *buf, size_t len)
{
	return -1;
}

ssize_t recv_socket(int fd, char *buf, size_t len)
{
	return -1;
}

void close_socket(int fd)
{
}

