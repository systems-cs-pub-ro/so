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

#include "../../include/lmc.h"

extern char *program_invocation_short_name;

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
	struct sockaddr_in server;

	if (name == NULL)
		name = program_invocation_short_name;

	strncpy(conn->name, name, LMC_CLIENT_MAX_NAME - 1);
	conn->name[LMC_CLIENT_MAX_NAME - 1] = 0;

	conn->socket = socket(AF_INET, SOCK_STREAM, 0);
	if (conn->socket < 0)
		return -1;

	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(LMC_SERVER_PORT);
	server.sin_addr.s_addr = inet_addr(LMC_SERVER_IP);

	if (connect(conn->socket , (struct sockaddr *)&server,
			sizeof(server)) < 0)
		return -1;

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
	close(conn->socket);
}
