/*
 * SO, 2016 - Lab 10 (Advanced I/O Windows)
 * Task #2 - Server
 */

/* do not use UNICODE */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <winsock2.h>

#include "sock_util.h"
#include "utils.h"

#define SERVER_LISTEN_PORT	43210
#define SERVER_BACKLOG		5

#define RECEIVING_FILE		"output.dat"

#ifndef BUFSIZ
#define BUFSIZ		4096
#endif

static CHAR gBuffer[BUFSIZ];

static HANDLE open_file(void)
{
	HANDLE hFile;

	hFile = CreateFile(
				RECEIVING_FILE,
				GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
	DIE(hFile == INVALID_HANDLE_VALUE, "CreateFile");

	return hFile;
}

static void receive_file(SOCKET hSocket, HANDLE hFile)
{
	int n;
	DWORD bytesWritten, dwRet;

	while (1) {
		n = recv(hSocket, gBuffer, BUFSIZ, 0);
		DIE(n == SOCKET_ERROR, "recv");

		if (n == 0)
			break;

		dwRet = WriteFile(hFile, gBuffer, n, &bytesWritten, NULL);
		DIE(dwRet == FALSE, "WriteFile");
	}
}

static void close_file(HANDLE hFile)
{
	CloseHandle(hFile);
}

int main(void)
{
	SOCKET hListen;
	SOCKET hSocket;
	HANDLE hFile;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);

	wsa_init();
	hFile = open_file();

	hListen = tcp_listen_connections(SERVER_LISTEN_PORT,
			SERVER_BACKLOG);
	DIE(hListen < 0, "tcp_listen_connections");

	hSocket = accept(hListen, (SSA *) &addr, &addrlen);
	print_connection(&addr);

	receive_file(hSocket, hFile);

	closesocket(hSocket);
	closesocket(hListen);
	wsa_cleanup();
	close_file(hFile);

	return 0;
}
