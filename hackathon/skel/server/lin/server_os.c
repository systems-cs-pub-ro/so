/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "../../include/server.h"

extern char *logfile_path;

static int client_function(SOCKET client_sock)
{
	int rc;
	struct logmemcache_client_st *client;

	client = logmemcache_create_client(client_sock);

	while (1) {
		rc = get_command(client);
		if (rc == -1)
			break;
	}

	close(client_sock);
	free(client);

	return 0;
}

void logmemcache_init_server_os(int *socket_server)
{
	int sock, client_size, client_sock;
	struct sockaddr_in server, client;

	memset(&server, 0, sizeof(struct sockaddr_in));

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return;

	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
		perror("Could not bind");
		exit(1);
	}

	if (listen(sock, DEFAULT_CLIENTS_NO) < 0) {
		perror("Error while listening");
		exit(1);
	}

	*socket_server = sock;

	while (1) {
		memset(&client, 0, sizeof(struct sockaddr_in));
		client_size = sizeof(struct sockaddr_in);
		client_sock = accept(sock, (struct sockaddr *)&client,
				(socklen_t *)&client_size);

		if (client_sock < 0) {
			perror("Error while accepting clients");
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
