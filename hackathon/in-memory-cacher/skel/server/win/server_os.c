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

char *lmc_logfile_path;

/**
 * Client connection loop function. Creates the appropriate client connection
 * socket and receives commands from the client in a loop.
 *
 * @param client_sock: Socket to communicate with the client.
 *
 * @return: This function always returns 0.
 *
 * TODO: The lmc_get_command function executes blocking operations. The server
 * is unable to handle multiple connections simultaneously.
 */
static DWORD WINAPI
lmc_client_function(SOCKET client_sock)
{
	int rc;
	struct lmc_client *client;

	client = lmc_create_client(client_sock);

	while (1) {
		rc = lmc_get_command(client);
		if (rc == -1)
			break;
	}

	closesocket(client_sock);
	free(client);

	return 0;
}

/**
 * Server main loop function. Opens a socket in listening mode and waits for
 * connections.
 */
void
lmc_init_server_os(void)
{
	SOCKET sock, client_sock;
	int client_size;
	struct sockaddr_in server, client;
	WSADATA wsaData;
	int rc;
	int opten;

	memset(&server, 0, sizeof(struct sockaddr_in));

	rc = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (rc != NO_ERROR)
		return;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	opten = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opten, sizeof(opten));

	server.sin_family = AF_INET;
	server.sin_port = htons(LMC_SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(LMC_SERVER_IP);

	if (bind(sock, (SOCKADDR *)&server, sizeof(server)) == SOCKET_ERROR) {
		closesocket(sock);
		perror("Could not bind");
		WSACleanup();
		exit(1);
	}

	if (listen(sock, LMC_DEFAULT_CLIENTS_NO) == SOCKET_ERROR) {
		perror("Error while listening");
		exit(1);
	}

	while (1) {
		memset(&client, 0, sizeof(struct sockaddr_in));
		client_size = sizeof(struct sockaddr_in);
		client_sock = accept(sock, (SOCKADDR *)&client,
				(socklen_t*)&client_size);
		if (client_sock == INVALID_SOCKET) {
			perror("Error while accepting clients");
			continue;
		}

		lmc_client_function(client_sock);
	}

}

/**
 * OS-specific client cache initialization function.
 *
 * @param cache: Cache structure to initialize.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
int
lmc_init_client_cache(struct lmc_cache *cache)
{
	return 0;
}

/**
 * OS-specific function that handles adding a log line to the cache.
 *
 * @param client: Client connection;
 * @param log: Log line to add to the cache.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic. Must be able to dynamically resize the
 * cache if it is full.
 */
int
lmc_add_log_os(struct lmc_client *client, struct lmc_client_logline *log)
{
	return 0;
}

/**
 * OS-specific function that handles flushing the cache to disk,
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
int
lmc_flush_os(struct lmc_client *client)
{
	return 0;
}

/**
 * OS-specific function that handles client unsubscribe requests.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic. Must flush the cache to disk and
 * deallocate any structures associated with the client.
 */
int
lmc_unsubscribe_os(struct lmc_client *client)
{
	return 0;
}
