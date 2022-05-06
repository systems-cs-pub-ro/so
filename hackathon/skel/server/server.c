/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/server.h"

#ifdef __unix__
#include <sys/socket.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

static struct lmc_cache **lmc_caches;
static size_t lmc_cache_count;
static size_t lmc_max_caches;

/* Server API */

/**
 * Initialize client cache list on the server.
 */
static void
lmc_init_client_list(void)
{
	lmc_max_caches = LMC_DEFAULT_CLIENTS_NO;
	lmc_caches = malloc(lmc_max_caches * sizeof(*lmc_caches));
}

/**
 * Initialize server - allocate initial cache list and start listening on the
 * server's socket.
 */
static void
lmc_init_server(void)
{
	lmc_init_client_list();
	lmc_init_server_os();
}

/**
 * Create a client connection structure. Called to allocate the client
 * connection structure by the code that handles the server loop.
 *
 * @param client_sock: Socket used to communicate with the client.
 *
 * @return: A pointer to a client connection structure on success,
 *          or NULL otherwise.
 */
struct lmc_client *
lmc_create_client(SOCKET client_sock)
{
	struct lmc_client *client;

	client = malloc(sizeof(*client));
	client->client_sock = client_sock;
	client->cache = NULL;

	return client;
}

/**
 * Handle client connect.
 *
 * Locate a cache entry for the client and allot it to the client connection
 * (populate the cache field of the client connection structure).
 * If the client already has an existing connection (and respective cache) use
 * the same cache. Otherwise, locate a free cache.
 *
 * @param client: Client connection;
 * @param name: The name (identifier) of the client.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Must be able to handle the case when all caches are occupied.
 */
static int
lmc_add_client(struct lmc_client *client, char *name)
{
	int err = 0;
	size_t i;

	for (i = 0; i < lmc_cache_count; i++) {
		if (lmc_caches[i] == NULL)
			continue;
		if (lmc_caches[i]->service_name == NULL)
			continue;
		if (strcmp(lmc_caches[i]->service_name, name) == 0) {
			client->cache = lmc_caches[i];
			goto found;
		}
	}

	if (lmc_cache_count == lmc_max_caches) {
		return -1;
	}

	client->cache = malloc(sizeof(*client->cache));
	client->cache->service_name = strdup(name);
	lmc_caches[lmc_cache_count] = client->cache;
	lmc_cache_count++;

	err = lmc_init_client_cache(client->cache);
found:
	return err;
}

/**
 * Handle client disconnect.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
static int
lmc_disconnect_client(struct lmc_client *client)
{
	return 0;
}

/**
 * Handle unsubscription requests.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
static int
lmc_unsubscribe_client(struct lmc_client *client)
{
	return 0;
}

/**
 * Add a log line to the client's cache.
 *
 * @param client: Client connection;
 * @param log: Log line to add to the cache.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
static int
lmc_add_log(struct lmc_client *client, struct lmc_client_logline *log)
{
	return 0;
}

/**
 * Flush client logs to disk.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
static int
lmc_flush(struct lmc_client *client)
{
	return 0;
}

/**
 * Send stats about the stored logs to the client. Must not send the actual
 * logs, but a string formatted in LMC_STATS_FORMAT. Should contain the current
 * time on the server, allocated memory in KBs and the number of log lines
 * stored.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
static int
lmc_send_stats(struct lmc_client *client)
{
	return 0;
}

/**
 * Send the stored log lines to the client.
 * The server must first send the number of lines, and then the log lines,
 * one by one.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 *
 * TODO: Implement proper handling logic.
 */
static int
lmc_send_loglines(struct lmc_client *client)
{
	return 0;
}

/**
 * Parse a command from the client. The command must be in the following format:
 * "cmd data", with a single space between the command and the associated data.
 *
 * @param cmd: Parsed command structure;
 * @param string: Command string;
 * @param datalen: The amount of data send with the command.
 */
static void
lmc_parse_command(struct lmc_command *cmd, char *string, ssize_t *datalen)
{
	char *command, *line;

	command = strdup(string);
	line = strchr(command, ' ');

	cmd->data = NULL;
	if (line != NULL) {
		line[0] = '\0';
		cmd->data = strdup(line + 1);
		*datalen -= strlen(command) + 1;
	}

	cmd->op = lmc_get_op_by_str(command);

	printf("command = %s, line = %s\n", cmd->op->op_str,
			cmd->data ? cmd->data : "null");

	free(command);
}

/**
 * Validate command argument. The command argument (data) can only contain
 * printable ASCII characters.
 *
 * @param line: Command data;
 * @param len: Length of the data string.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
static int
lmc_validate_arg(const char *line, size_t len)
{
	size_t i;

	for (i = 0; i < len; i++)
		if (!isprint(line[i]))
			return -1;

	return 0;
}

/**
 * Wait for a command from the client and handle it when it is received.
 * The server performs blocking receive operations in this function. When the
 * command is received, parse it and then call the appropriate handling
 * function, depending on the command.
 *
 * @param client: Client connection.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_get_command(struct lmc_client *client)
{
	int err;
	ssize_t recv_size;
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	char *reply_msg;
	struct lmc_command cmd;
	struct lmc_client_logline *log;

	err = -1;

	memset(&cmd, 0, sizeof(cmd));
	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	recv_size = lmc_recv(client->client_sock, buffer, sizeof(buffer), 0);
	if (recv_size <= 0)
		return -1;

	lmc_parse_command(&cmd, buffer, &recv_size);
	if (recv_size > LMC_LINE_SIZE) {
		reply_msg = "message too long";
		goto end;
	}

	if (cmd.op->requires_auth && client->cache->service_name == NULL) {
		reply_msg = "authentication required";
		goto end;
	}

	if (cmd.data != NULL) {
		err = lmc_validate_arg(cmd.data, recv_size);
		if (err != 0) {
			reply_msg = "invalid argument provided";
			goto end;
		}
	}

	switch (cmd.op->code) {
	case LMC_CONNECT:
	case LMC_SUBSCRIBE:
		err = lmc_add_client(client, cmd.data);
		break;
	case LMC_STAT:
		err = lmc_send_stats(client);
		break;
	case LMC_ADD:
		/* TODO parse the client data and create a log line structure */
		log = NULL;
		err = lmc_add_log(client, log);
		break;
	case LMC_FLUSH:
		err = lmc_flush(client);
		break;
	case LMC_DISCONNECT:
		err = lmc_disconnect_client(client);
		break;
	case LMC_UNSUBSCRIBE:
		err = lmc_unsubscribe_client(client);
		break;
	case LMC_GETLOGS:
		err = lmc_send_loglines(client);
		break;
	default:
		/* unknown command */
		err = -1;
		break;
	}

	reply_msg = cmd.op->op_reply;

end:
	if (err == 0)
		sprintf(response, "%s", reply_msg);
	else
		sprintf(response, "FAILED: %s", reply_msg);

	if (cmd.data != NULL)
		free(cmd.data);

	return lmc_send(client->client_sock, response, LMC_LINE_SIZE,
			LMC_SEND_FLAGS);
}

int
main(int argc, char *argv[])
{
	setbuf(stdout, NULL);

	if (argc == 1)
		lmc_logfile_path = strdup("logs_lmc");
	else
		lmc_logfile_path = strdup(argv[1]);

	if (lmc_init_logdir(lmc_logfile_path) < 0)
		exit(-1);

	lmc_init_server();

	return 0;
}
