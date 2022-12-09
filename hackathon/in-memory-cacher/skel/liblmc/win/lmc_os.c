/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#undef _UNICODE
#undef UNICODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include "../../include/lmc.h"

#include <windows.h>
#include <Psapi.h>
#include <shlwapi.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

/**
 * OS-specific code that connects to the server
 *
 * @param conn: Connection to the server;
 * @param name: The name (identifier) of the client.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_conn_init_os(struct lmc_conn *conn, char *name)
{
	WSADATA wsaData;
	HANDLE currentProcess;
	struct sockaddr_in server;
	char *process_name;
	size_t name_len;
	int rc;

	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc != NO_ERROR)
		return -1;

	if (name == NULL) {
		currentProcess = GetCurrentProcess();
		process_name = calloc(MAX_PATH, sizeof(char));
		name_len = GetProcessImageFileNameA(currentProcess,
				process_name, MAX_PATH);
		PathStripPath(process_name);
		if (!strcmp(process_name + (strlen(process_name) - 4), ".exe"))
			process_name[strlen(process_name) - 4] = '\0';
	} else {
		process_name = name;
	}

	strncpy(conn->name, process_name, LMC_CLIENT_MAX_NAME - 1);
	conn->name[LMC_CLIENT_MAX_NAME - 1] = 0;

	conn->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (conn->socket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(LMC_SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(LMC_SERVER_IP);

	if (connect(conn->socket , (SOCKADDR *)&server,
			sizeof(server)) == SOCKET_ERROR) {
		closesocket(conn->socket);
		WSACleanup();
		return -1;
	}

	return 0;
}

/**
 * OS-specific code that frees / disconnects from the server.
 *
 * @param conn: Connection to the server.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
void
lmc_conn_free_os(struct lmc_conn *conn)
{
	closesocket(conn->socket);
	WSACleanup();

	return;
}
