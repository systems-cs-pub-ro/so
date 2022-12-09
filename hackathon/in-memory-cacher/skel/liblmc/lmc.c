/**
 * Hackathon SO: LogMemCacher
 * (c) 2020-2021, Operating Systems
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/lmc.h"

#ifdef _WIN32
#include <windows.h>
#endif

/* Client API */

/**
 * Initialize a connection to the server.
 *
 * @param conn: Connection to the server;
 * @param name: The name (identifier) of the client.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
static int
lmc_conn_init(struct lmc_conn *conn, char *name)
{
	int err;
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	const struct lmc_op *op;
	size_t len;

	err = lmc_conn_init_os(conn, name);
	if (err != 0)
		return err;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = lmc_get_op(LMC_CONNECT);
	len = snprintf(buffer, sizeof(buffer),
		"%s %s", op->op_str, conn->name);

	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while connecting to lmcd\n");
		return -1;
	}

	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting response from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

/**
 * Connect to the server.
 *
 * @param name: The name (identifier) of the client.
 *
 * @return: A pointer to a connection descriptor in case of success, or NULL
 *          otherwise.
 */
struct lmc_conn *
lmc_connect(char *name)
{
	struct lmc_conn *conn;

	conn = malloc(sizeof(struct lmc_conn));
	if (conn != NULL) {
		conn->name = malloc(LMC_CLIENT_MAX_NAME * sizeof(char));
		if (lmc_conn_init(conn, name) < 0) {
			fprintf(stderr, "Could not allocate conn\n");
			return NULL;
		}
	}

	return conn;
}

/**
 * Free / close a connection to the server.
 *
 * @param conn: Connection to the server to free.
 */
void
lmc_free(struct lmc_conn *conn)
{
	lmc_conn_free_os(conn);
	free(conn);
}

/**
 * Request storing a log line on the server.
 *
 * @param conn: Connection to the server;
 * @param logline: Information to store in the cache.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_send_log(struct lmc_conn *conn, char *logline)
{
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	char time[LMC_TIME_SIZE];
	size_t len;
	const struct lmc_op *op;

	lmc_crttime_to_str(time, LMC_TIME_SIZE, LMC_TIME_FORMAT);

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = lmc_get_op(LMC_ADD);
	len = snprintf(buffer, sizeof(buffer),
		"%s %s:%s", op->op_str, time, logline);

	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while adding logline to lmcd\n");
		return -1;
	}

	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting response from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

/**
 * Request flushing the cache on the server to disk.
 *
 * @param conn: Connection to the server.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_flush(struct lmc_conn *conn)
{
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	const struct lmc_op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = lmc_get_op(LMC_FLUSH);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);

	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while sending flush to lmcd\n");
		return -1;
	}

	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while getting response from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

/**
 * Retrieve the logs for the current server from the server.
 *
 * @param conn: Connection to the server;
 * @param t1: Beginning time (retrieve only logs newer than this time);
 * @param t2: Ending time (retrieve only logs older than this time);
 * @param logs: Number of logs received from the server.
 *
 * @return: A list of logs received from the server. Is NULL if there are no
 * logs to retrieve or in case of an error.
 */
struct lmc_client_logline **
lmc_get_logs(struct lmc_conn *conn, time_t t1, time_t t2, uint64_t *logs)
{
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	struct lmc_client_logline **lines;
	uint64_t num_logs, i;
	const struct lmc_op *op;
	size_t len;
	int rc;

	memset(buffer, 0, sizeof(buffer));

	op = lmc_get_op(LMC_GETLOGS);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while getting logs from server\n");
		return NULL;
	}
	num_logs = 0;

	memset(response, 0, sizeof(response));
	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0) {
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
		if (lmc_recv(conn->socket, lines[i],
				sizeof(*lines[i]), 0) < 0) {
			goto err;
		}
	}

	memset(response, 0, sizeof(response));
	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0)
		fprintf(stderr, "error while getting response from server\n");
	else
		fprintf(stdout, "%s\n", response);

err:
	*logs = i;
	return lines;
}

/**
 * Retrieve stats about the logs stored on the server.
 *
 * @param conn: Connection to the server;
 *
 * @return: A string with the stats' description on success, or NULL otherwise.
 */
char *
lmc_get_stats(struct lmc_conn *conn)
{
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	char *data;
	const struct lmc_op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	data = calloc(LMC_STATUS_MAX_SIZE, sizeof(char));

	op = lmc_get_op(LMC_STAT);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while getting status from server\n");
	}

	if (lmc_recv(conn->socket, data, LMC_STATUS_MAX_SIZE, 0) < 0) {
		fprintf(stderr, "Error while getting status from server\n");
		data = NULL;
	}

	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0)
		fprintf(stderr, "Error while getting response from server\n");
	else
		fprintf(stdout, "%s\n", response);

	return data;
}

/**
 * Send a disconnect request to the server.
 *
 * @param conn: Connection to the server.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_disconnect(struct lmc_conn *conn)
{
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	const struct lmc_op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = lmc_get_op(LMC_DISCONNECT);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while deconnecting from server\n");
		return -1;
	}

	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while disconnecting from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

/**
 * Send an unsubscription request to the server.
 *
 * @param conn: Connection to the server.
 *
 * @return: 0 in case of success, or -1 otherwise.
 */
int
lmc_unsubscribe(struct lmc_conn *conn)
{
	char buffer[LMC_COMMAND_SIZE], response[LMC_LINE_SIZE];
	const struct lmc_op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));
	memset(response, 0, sizeof(response));

	op = lmc_get_op(LMC_UNSUBSCRIBE);
	len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while unsubscribing from server\n");
		return -1;
	}

	if (lmc_recv(conn->socket, response, sizeof(response), 0) < 0) {
		fprintf(stderr, "Error while unsubscribing from server\n");
		return -1;
	}
	fprintf(stdout, "%s\n", response);

	return 0;
}

/**
 * Free a buffer allocated in the library's heap. This is required since the
 * Win32 allocates data in a different heap area in libraries.
 * This function should be called on buffers allocated by lmc_get_stats or
 * lmc_get_logs.
 *
 * @param buf: Pointer to buffer to free.
 */
void
lmc_free_buf(void *buf)
{
	free(buf);
}
