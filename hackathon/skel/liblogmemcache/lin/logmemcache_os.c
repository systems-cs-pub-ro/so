/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#include "../../include/logmemcache.h"

extern char *program_invocation_short_name;

int logmemcache_client_init_os(struct logmemcache_st *client, char *name)
{
	struct sockaddr_in server;

	if (name == NULL)
		name = program_invocation_short_name;

	strncpy(client->name, name, CLIENT_MAX_NAME - 1);
	client->name[CLIENT_MAX_NAME - 1] = 0;

	client->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->socket < 0)
		return -1;

	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(client->socket , (struct sockaddr *)&server,
			sizeof(server)) < 0)
		return -1;

	return 0;
}

void logmemcache_client_free_os(struct logmemcache_st *client)
{
	close(client->socket);
}
