/*
 * SO, 2016 - Lab 10 (Advanced I/O Windows)
 * Task #2 - Client
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>

#include "sock_util.h"
#include "utils.h"

#define SERVER_LISTEN_PORT	43210


static HANDLE open_file(const char *filename)
{
	HANDLE hFile;

	hFile = CreateFile(
				filename,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
				NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");

	return hFile;
}

static void transmit_file(SOCKET hSocket, HANDLE hFile)
{
	DWORD dwRet;

	/* TODO - send file to server */
}

static void close_file(HANDLE hFile)
{
	CloseHandle(hFile);
}

int main(int argc, char **argv)
{
	SOCKET connectfd;
	HANDLE hFile;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s input_file\n", argv[0]);
		return -1;
	}

	hFile = open_file(argv[1]);
	wsa_init();

	connectfd = tcp_connect_to_server("localhost", SERVER_LISTEN_PORT);
	if (connectfd < 0)
		ExitProcess(1);

	transmit_file(connectfd, hFile);

	closesocket(connectfd);
	wsa_cleanup();
	close_file(hFile);

	return 0;
}
