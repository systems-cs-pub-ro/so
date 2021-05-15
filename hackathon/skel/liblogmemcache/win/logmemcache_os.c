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
#include "../../include/logmemcache.h"

#include <windows.h>
#include <Psapi.h>
#include <shlwapi.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

int logmemcache_client_init_os(struct logmemcache_st *client, char *name)
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
						process_name,
						MAX_PATH);
		PathStripPath(process_name);
		if (!strcmp(process_name + (strlen(process_name) - 4), ".exe"))
			process_name[strlen(process_name) - 4] = '\0';
	} else {
		process_name = name;
	}

	strncpy(client->name, process_name, CLIENT_MAX_NAME - 1);
	client->name[CLIENT_MAX_NAME - 1] = 0;

	client->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->socket == INVALID_SOCKET) {
		WSACleanup();
		return -1;
	}

	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(client->socket , (SOCKADDR *)&server,
			sizeof(server)) == SOCKET_ERROR) {
		closesocket(client->socket);
		WSACleanup();
		return -1;
	}

	return 0;
}

void logmemcache_client_free_os(struct logmemcache_st *client)
{
	closesocket(client->socket);
	WSACleanup();

	return;
}
