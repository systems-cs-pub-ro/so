#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __unix__
#include <signal.h>
#endif

#include "logmemcache.h"


#define STREQ(S1, S2)		(strcmp(S1, S2) == 0)
#define PADLEN			80
#define SHORT_LINE_LEN		20
#define LONG_LINE_LEN		200
#define TOO_BIG_LINE_LEN	(LOGLINE_SIZE * 2)

struct test_desc {
	int (*cb)(const void *);
	const char *tag;
	const char *desc;
};


static void fill_chars(char *buf, size_t len, int no_nonprint)
{
	size_t i;
	char c;

	for (i = 0; i < len; i++) {
		do {
			c = rand() % 256;
		} while (no_nonprint && !isprint(c));

		buf[i] = c;
	}
}

static int client_add_lines(struct logmemcache_st *client, int count,
	size_t line_len, int no_nonprint)
{
	int i, ret;
	char *line;

	ret = 0;

	line = malloc(line_len * sizeof(char));
	if (line == NULL)
		return -1;

	for (i = 0; i < count; i++) {
		fill_chars(line, line_len, no_nonprint);
		line[line_len - 1] = '\0';

		ret = logmemcache_add_log(client, line);
		if (ret != 0)
			break;
	}

	free(line);

	return ret;
}

static int print_client_stats(struct logmemcache_st *client)
{
	char *stats;

	stats = logmemcache_get_stats(client);
	if (stats == NULL) {
		printf("Failed to get stats\n");
		return -1;
	}

	printf("%s\n", stats);
	free(stats);

	return 0;
}

static int print_client_logs(struct logmemcache_st *client)
{
	char time[TIME_SIZE + 1];
	char logline[LINE_SIZE - TIME_SIZE + 2];
	struct client_logline **lines;
	uint64_t count, i;
	int ret;

	lines = logmemcache_get_logs(client, 0, 0, &count);
	printf("Server reported " UINT64_FMT " logs\n", count);

	for (i = 0; i < count; i++) {
		ret = logmemcache_get_time(time, lines[i], sizeof(time));
		if (ret != 0) {
			printf("Failed to parse time\n");
			break;
		}

		ret = logmemcache_get_logline(logline, lines[i],
					sizeof(logline));
		if (ret != 0) {
			printf("Failed to parse logline\n");
			break;
		}

		printf("%s: %s\n", time, logline);
	}

	if (lines != NULL) {
		for (i = 0; i < count; i++)
			if (lines[i] != NULL)
				free(lines[i]);
		free(lines);
	}

	return ret;
}

static int do_incomplete_op(struct logmemcache_st *client, enum op_code code)
{
#ifdef __unix__
	char buffer[COMMAND_SIZE], time[TIME_SIZE], logline[120];
	const struct op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));

	op = get_op(code);
	if (code == ADD) {
		fill_chars(logline, sizeof(logline), 1);
		current_time_to_string(time, TIME_SIZE, TIME_FORMAT);

		len = snprintf(buffer, sizeof(buffer),
			"%s %s%s", op->op_str, time, logline);
	} else {
		len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	}

	if (send_data(client->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while sending flush to logmemcached\n");
		return -1;
	}
#endif

	return 0;
}

static int basic_test_1(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Connect error\n");
		return 0;
	}

	printf("Client connected successfully\n");
	logmemcache_free(client);

	return 1;
}

static int basic_test_2(const void *param)
{
	struct logmemcache_st *clients[10];
	const struct test_desc *test = param;
	int i, ret;

	ret = 0;

	for (i = 0; i < nitems(clients); i++) {
		clients[i] = logmemcache_create((char *)test->tag);
		if (!clients[i]) {
			printf("Connect error\n");
			ret = -1;
			break;
		}
	}

	for (i = i - 1; i >= 0; i--)
		logmemcache_free(clients[i]);

	return ret == 0;
}

static int basic_test_3(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Connect error\n");
		return 0;
	}

	ret = logmemcache_disconnect(client);
	if (ret == -1)
		printf("Disconnect error\n");

	logmemcache_free(client);

	return ret == 0;
}

static int basic_test_4(const void *param)
{
	struct logmemcache_st *clients[10];
	const struct test_desc *test = param;
	int i, ret;

	for (i = 0; i < nitems(clients); i++) {
		clients[i] = logmemcache_create((char *)test->tag);
		if (!clients[i]) {
			printf("Connect error\n");
			ret = -1;
			break;
		}

		ret = logmemcache_disconnect(clients[i]);
		if (ret != 0) {
			printf("Disconnect error\n");
			break;
		}
	}

	for (i = i - 1; i >= 0; i--)
		logmemcache_free(clients[i]);

	return ret == 0;
}

static int op_test_1(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_2(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 30, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_3(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, LONG_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_4(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 20, LONG_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_5(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_6(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = logmemcache_flush(client);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_7(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = print_client_logs(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int op_test_8(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

#ifdef __unix__
	signal(SIGPIPE, SIG_IGN);
#endif

	ret = logmemcache_disconnect(client);

#ifdef __unix__
	signal(SIGPIPE, SIG_DFL);
#endif
	if (ret == 0) {
		printf("Should not be able to disconnect twice\n");
		ret = -1;
	} else {
		ret = 0;
	}

	logmemcache_free(client);

	return ret == 0;
}

static int op_test_9(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = logmemcache_unsubscribe(client);
	if (ret != 0) {
		printf("Unsubscribe error\n");
		goto done;
	}

#ifdef __unix__
	signal(SIGPIPE, SIG_IGN);
#endif

	ret = logmemcache_disconnect(client);

#ifdef __unix__
	signal(SIGPIPE, SIG_DFL);
#endif
	if (ret == 0) {
		printf("Connection should be closed after unsubscribe\n");
		ret = -1;
	} else {
		ret = 0;
	}

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_1(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_2(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 2, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_3(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 20, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_4(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_flush(client);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_5(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_flush(client);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_6(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_logs(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_7(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 2, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_logs(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_8(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 20, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_logs(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int combo_test_9(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_logs(client);
	if (ret != 0)
		goto done;

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = client_add_lines(client, 3, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_logs(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_flush(client);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = client_add_lines(client, 3, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = print_client_stats(client);
	if (ret != 0)
		goto done;

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int incom_test_1(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(client, ADD);
	logmemcache_free(client);

	return ret == 0;
}

static int incom_test_2(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(client, STAT);
	logmemcache_free(client);

	return ret == 0;
}

static int incom_test_3(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(client, FLUSH);
	logmemcache_free(client);

	return ret == 0;
}

static int incom_test_4(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(client, GETLOGS);
	logmemcache_free(client);

	return ret == 0;
}

static int incom_test_5(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(client, DISCONNECT);
	logmemcache_free(client);

	return ret == 0;
}

static int incom_test_6(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(client, UNSUBSCRIBE);
	logmemcache_free(client);

	return ret == 0;
}

static int inv_test_1(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, LONG_LINE_LEN, 0);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}

static int inv_test_2(const void *param)
{
	struct logmemcache_st *client;
	const struct test_desc *test = param;
	int ret;

	client = logmemcache_create((char *)test->tag);
	if (!client) {
		printf("Client not connected\n");
		return 0;
	}

	ret = client_add_lines(client, 1, TOO_BIG_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	ret = logmemcache_disconnect(client);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	logmemcache_free(client);

	return ret == 0;
}


static const struct test_desc test_descs[] = {
	{ basic_test_1,  "basic-1",  "Client can connect to server" },
	{ basic_test_2,  "basic-2",  "Repeating connects from same client" },
	{ basic_test_3,  "basic-3",  "Connect and disconnect" },
	{ basic_test_4,  "basic-4",  "Repeating connects and disconnects" },

	{ op_test_1,     "op-1",     "Connect, add, disconnect" },
	{ op_test_2,     "op-2",     "Connect, many adds, disconnect" },
	{ op_test_3,     "op-3",     "Connect, add long line, disconnect" },
	{ op_test_4,     "op-4",     "Connect, add long lines, disconnect" },
	{ op_test_5,     "op-5",     "Connect, stat, disconnect" },
	{ op_test_6,     "op-6",     "Connect, flush, disconnect" },
	{ op_test_7,     "op-7",     "Connect, getlogs, disconnect" },
	{ op_test_8,     "op-8",     "Connect, disconnect, disconnect" },
	{ op_test_9,     "op-9",     "Connect, unsubscribe, disconnect" },

	{ combo_test_1,  "combo-1",  "Connect, add, stat, disconnect" },
	{ combo_test_2,  "combo-2",  "Connect, 2x add, stat, disconnect" },
	{ combo_test_3,  "combo-3",  "Connect, many adds, stat, disconnect" },
	{ combo_test_4,  "combo-4",  "Connect, add, flush, disconnect" },
	{ combo_test_5,  "combo-5",  "Connect, add, stat, flush, disconnect" },
	{ combo_test_6,  "combo-6",  "Connect, add, getlogs, disconnect" },
	{ combo_test_7,  "combo-7",  "Connect, 2x adds, getlogs, disconnect" },
	{ combo_test_8,  "combo-8",  "Connect, many adds, getlogs, disconnect" },
	{ combo_test_9,  "combo-9",  "Many ops" },

	{ incom_test_1,  "incom-1",  "Connect, incomplete add comm" },
	{ incom_test_2,  "incom-2",  "Connect, incomplete stat comm" },
	{ incom_test_3,  "incom-3",  "Connect, incomplete flush comm" },
	{ incom_test_4,  "incom-4",  "Connect, incomplete getlogs comm" },
	{ incom_test_5,  "incom-5",  "Connect, incomplete disconnect comm" },
	{ incom_test_6,  "incom-6",  "Connect, incomplete unsubscribe comm" },

	{ inv_test_1,    "inv-1",    "Connect, add line with non-printable characters" },
	{ inv_test_2,    "inv-2",    "Connect, add line too big, disconnect" },
};

static void run_test(const struct test_desc *test)
{
	int success;
	char *msg;
	char padding[PADLEN];
	int len;

	memset(padding, '=', sizeof(padding));
	padding[sizeof(padding) - 1] = '\0';

	printf("[\x1b[0;33mTest %s: %s\x1b[0m]\n", test->tag, test->desc);

	success = test->cb(test);
	msg = success ? "\x1b[0;32mSUCCESS\x1b[0m" : "\x1b[0;31mFAIL\x1b[0m";

	len = sizeof(padding) - (int)strlen(msg) - 1;
	printf("%s %.*s\n", msg, len, padding);
}

int main(int argc, char *argv[])
{
	int i, rc;
	const char *tag;
	const struct test_desc *test;

	setbuf(stdout, NULL);
	srand(time(NULL));

	rc = 0;
	tag = "all";
	if (argc >= 2) {
		tag = argv[1];
		rc = sscanf(tag, "%d", &i);
	}

	if (rc != 0) {
		if (i >= nitems(test_descs) + 1) {
			fprintf(stderr, "Invalid index: %d\n", i);
			exit(1);
		}

		test = &test_descs[i - 1];
		run_test(test);
	} else {
		for (i = 0; i < nitems(test_descs); i++) {
			test = &test_descs[i];
			if (STREQ(tag, "all") || STREQ(tag, test->tag))
				run_test(test);
		}
	}

	return 0;
}
