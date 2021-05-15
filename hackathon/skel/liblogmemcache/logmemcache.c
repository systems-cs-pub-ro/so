/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/logmemcache.h"

#ifdef _WIN32
#include <windows.h>
#endif

/* Client API */

static int logmemcache_client_init(struct logmemcache_st *client, char *name)
{
	int err;
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	const struct op *op;
	size_t len;

	err = logmemcache_client_init_os(client, name);
	if (err != 0)
		return err;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = get_op(CONNECT);
	len = snprintf(buffer, sizeof(buffer),
		"%s %s", op->op_str, client->name);

	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while connecting to logmemcached\n");
		return -1;
	}

	if (recv_data(client->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting response from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

struct logmemcache_st *logmemcache_create(char *name)
{
	struct logmemcache_st *client;

	client = malloc(sizeof(struct logmemcache_st));
	if (client != NULL) {
		client->name = malloc(CLIENT_MAX_NAME * sizeof(char));
		if (logmemcache_client_init(client, name) < 0) {
			fprintf(stderr, "Could not allocate client\n");
			return NULL;
		}
	}

	return client;
}

void logmemcache_free(struct logmemcache_st *client)
{

	logmemcache_client_free_os(client);
	free(client);
}

int logmemcache_add_log(struct logmemcache_st *client, char *logline)
{
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	char time[TIME_SIZE];
	size_t len;
	const struct op *op;

	current_time_to_string(time, TIME_SIZE, TIME_FORMAT);

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = get_op(ADD);
	len = snprintf(buffer, sizeof(buffer),
		"%s %s%s", op->op_str, time, logline);

	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while adding logline to logmemcached\n");
		return -1;
	}

	if (recv_data(client->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting response from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

int logmemcache_flush(struct logmemcache_st *client)
{
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	const struct op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = get_op(FLUSH);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);

	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while sending flush to logmemcached\n");
		return -1;
	}

	if (recv_data(client->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting response from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

struct client_logline **logmemcache_get_logs(struct logmemcache_st *client,
	time_t t1, time_t t2, uint64_t *logs)
{
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	struct client_logline **lines;
	uint64_t num_logs, i;
	const struct op *op;
	size_t len;
	int rc;

	memset(buffer, 0, sizeof(buffer));

	op = get_op(GETLOGS);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while getting logs from server\n");
		return NULL;
	}
	num_logs = 0;

	memset(response, 0, sizeof(response));
	if (recv_data(client->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting status from server\n");
		return NULL;
	}

	rc = sscanf(response, UINT64_FMT, &num_logs);
	if (rc == 0)
		return NULL;

	if (num_logs != 0)
		lines = calloc((size_t)num_logs, sizeof(*lines));
	else
		lines = NULL;

	for (i = 0; i < num_logs; i++) {
		lines[i] = calloc(1, sizeof(*lines[i]));
		if (recv_data(client->socket, lines[i],
				sizeof(*lines[i]), 0) < 0) {
			goto err;
		}
	}

	memset(response, 0, sizeof(response));
	if (recv_data(client->socket, response, sizeof(response), 0) < 0)
		fprintf(stderr, "error while getting response from server\n");
	else
		fprintf(stdout, "%s\n", response);

err:
	*logs = i;
	return lines;
}

char *logmemcache_get_stats(struct logmemcache_st *client)
{
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	char *data;
	const struct op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	data = calloc(STATUS_MAX_SIZE, sizeof(char));

	op = get_op(STAT);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while getting status from server\n");
	}

	if (recv_data(client->socket, data, STATUS_MAX_SIZE, 0) < 0) {
		fprintf(stderr, "Error while getting status from server\n");
		data = NULL;
	}

	if (recv_data(client->socket, response, sizeof(response), 0) < 0)
		fprintf(stderr, "Error while getting response from server\n");
	else
		fprintf(stdout, "%s\n", response);

	return data;
}

int logmemcache_disconnect(struct logmemcache_st *client)
{
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	const struct op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = get_op(DISCONNECT);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while deconnecting from server\n");
		return -1;
	}

	if (recv_data(client->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while disconnecting from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

int logmemcache_unsubscribe(struct logmemcache_st *client)
{
	char buffer[COMMAND_SIZE], response[LINE_SIZE];
	const struct op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = get_op(UNSUBSCRIBE);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while unsubscribing from server\n");
		return -1;
	}

	if (recv_data(client->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while unsubscribing from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

int logmemcache_get_time(char *time, struct client_logline *line, size_t len)
{
	if (len < TIME_SIZE)
		return -1;

	strncpy(time, line->time, TIME_SIZE - 1);
	time[TIME_SIZE - 1] = '\0';

	return 0;
}

int logmemcache_get_logline(char *logline, struct client_logline *line,
	size_t len)
{
	if (len <= LOGLINE_SIZE)
		return -1;

	strncpy(logline, (char *) ((long)line->logline - 1), LOGLINE_SIZE);
	logline[LOGLINE_SIZE] = '\0';

	return 0;
}
