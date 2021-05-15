/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib,"Ws2_32.lib")

#include "../../include/server.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

extern char *logfile_path;

DWORD WINAPI client_function(SOCKET client_sock)
{
	int rc;
	struct logmemcache_client_st *client;

	client = logmemcache_create_client(client_sock);

	while (1) {
		rc = get_command(client);
		if (rc == -1)
			break;
	}

	closesocket(client_sock);
	free(client);

	return 0;
}

void logmemcache_init_server_os(SOCKET *server_socket)
{
	SOCKET sock, client_sock;
	int client_size;
	struct sockaddr_in server, client;
	WSADATA wsaData;
	int rc;

	memset(&server, 0, sizeof(struct sockaddr_in));

	rc = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (rc != NO_ERROR)
		return;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (bind(sock, (SOCKADDR *)&server, sizeof(server)) == SOCKET_ERROR) {
		closesocket(sock);
		perror("Could not bind");
		WSACleanup();
		exit(1);
	}

	if (listen(sock, DEFAULT_CLIENTS_NO) == SOCKET_ERROR) {
		perror("Error while listening");
		exit(1);
	}

	*server_socket = sock;

	while (1) {
		memset(&client, 0, sizeof(struct sockaddr_in));
		client_size = sizeof(struct sockaddr_in);
		client_sock = accept(sock, (SOCKADDR *)&client,
				(socklen_t*)&client_size);
		if (client_sock == INVALID_SOCKET) {
			perror("Error while accepting clients");
			continue;
		}

		client_function(client_sock);
	}

}

int logmemcache_init_client_cache(struct logmemcache_cache *cache)
{

	return 0;
}

int logmemcache_add_log_os(struct logmemcache_client_st *client,
	struct client_logline *log)
{

	return 0;
}

int logmemcache_flush_os(struct logmemcache_client_st *client)
{

	return 0;
}

int logmemcache_unsubscribe_os(struct logmemcache_client_st *client)
{

	return 0;
}
