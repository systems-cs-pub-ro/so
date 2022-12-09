#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __unix__
#include <signal.h>
#endif

#include "lmc.h"

#define VERBOSE	 0


#define STREQ(S1, S2)		(strcmp(S1, S2) == 0)
#define PADLEN			80
#define SHORT_LINE_LEN		20
#define LONG_LINE_LEN		200
#define TOO_BIG_LINE_LEN	(LMC_LOGLINE_SIZE * 2)

#define MAX_LOG_LINES		100

struct test_desc {
	int (*cb)(const void *);
	const char *tag;
	const char *desc;
};


static struct sent_loglines {
	time_t time;
	char timestr[LMC_TIME_SIZE];
	char data[LMC_LOGLINE_SIZE];
} sent_logs[MAX_LOG_LINES];

static int sent_log_idx;


static void
fill_chars(char *buf, size_t len, int no_nprint)
{
	size_t i;
	char c;

	for (i = 0; i < len; i++) {
		do {
			c = rand() % 256;
		} while (no_nprint && !isprint(c));

		buf[i] = c;
	}
}

static int
add_log_lines(struct lmc_conn *conn, uint64_t count, size_t line_len,
	int no_nprint)
{
	int i, ret;
	char *line;

	ret = 0;

	if (count > MAX_LOG_LINES) {
		fprintf(stderr, "Too many log lines in test!\n");
		return -1;
	}

	line = malloc(line_len * sizeof(char));
	if (line == NULL)
		return -1;

	for (i = 0; i < count; i++) {
		fill_chars(line, line_len, no_nprint);
		line[line_len - 1] = '\0';

		/*
		 * The timestamp could also be saved / we could use custom
		 * timestamps, but the send function would have to be
		 * redefined.
		 */
		if (line_len < LMC_LOGLINE_SIZE)
			strcpy(sent_logs[sent_log_idx++].data, line);

		ret = lmc_send_log(conn, line);
		if (ret != 0)
			break;
	}

	free(line);

	return ret;
}

static int
print_stats(struct lmc_conn *conn)
{
	char *stats;

	stats = lmc_get_stats(conn);
	if (stats == NULL) {
		printf("Failed to get stats\n");
		return -1;
	}

	if (VERBOSE)
		printf("%s\n", stats);

	lmc_free_buf(stats);

	return 0;
}

static uint64_t
count_log_lines(struct lmc_conn *conn)
{
	uint64_t count;
	lmc_get_logs(conn, 0, 0, &count);
	return count;
}

static int
test_log_lines(struct lmc_conn *conn)
{
	struct lmc_client_logline **lines;
	uint64_t count, i;
	int ret;

	ret = 0;
	lines = lmc_get_logs(conn, 0, 0, &count);
	printf("Server reported " UINT64_FMT " logs\n", count);

	for (i = 0; i < count; i++) {
		if (!STREQ(lines[i]->logline, sent_logs[i].data)) {
			fprintf(stderr, "Unexpected line: %s. Expected: %s\n",
				lines[i]->logline, sent_logs[i].data);
			ret = -1;
		}

		if (VERBOSE)
			printf("%s: %s\n", lines[i]->time, lines[i]->logline);
	}

	if (lines != NULL) {
		for (i = 0; i < count; i++)
			if (lines[i] != NULL)
				lmc_free_buf(lines[i]);
		lmc_free_buf(lines);
	}

	return ret;
}

static int
do_incomplete_op(struct lmc_conn *conn, enum lmc_op_code code)
{
	char buffer[LMC_COMMAND_SIZE], time[LMC_TIME_SIZE], logline[120];
	const struct lmc_op *op;
	size_t len;

	memset(buffer, 0, sizeof(buffer));

	op = lmc_get_op(code);
	if (code == LMC_ADD) {
		fill_chars(logline, sizeof(logline), 1);
		lmc_crttime_to_str(time, LMC_TIME_SIZE, LMC_TIME_FORMAT);

		len = snprintf(buffer, sizeof(buffer),
			"%s %s%s", op->op_str, time, logline);
	} else {
		len = snprintf(buffer, sizeof(buffer), "%s", op->op_str);
	}

	if (lmc_send(conn->socket, buffer, len, 0) < 0) {
		fprintf(stderr, "Error while sending flush to lmcd\n");
		return -1;
	}

	return 0;
}

static int
basic_test_1(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Connect error\n");
		return 0;
	}

	printf("Client connected successfully\n");
	lmc_free(conn);

	return 1;
}

static int
basic_test_2(const void *param)
{
	struct lmc_conn *conns[10];
	const struct test_desc *test = param;
	int i, ret;

	ret = 0;

	for (i = 0; i < nitems(conns); i++) {
		conns[i] = lmc_connect((char *)test->tag);
		if (!conns[i]) {
			printf("Connect error\n");
			ret = -1;
			break;
		}
	}

	for (i = i - 1; i >= 0; i--)
		lmc_free(conns[i]);

	return ret == 0;
}

static int
basic_test_3(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Connect error\n");
		return 0;
	}

	ret = lmc_disconnect(conn);
	if (ret == -1)
		printf("Disconnect error\n");

	lmc_free(conn);

	return ret == 0;
}

static int
basic_test_4(const void *param)
{
	struct lmc_conn *conns[10];
	const struct test_desc *test = param;
	int i, ret;

	for (i = 0; i < nitems(conns); i++) {
		conns[i] = lmc_connect((char *)test->tag);
		if (!conns[i]) {
			printf("Connect error\n");
			ret = -1;
			break;
		}

		ret = lmc_disconnect(conns[i]);
		if (ret != 0) {
			printf("Disconnect error\n");
			break;
		}
	}

	for (i = i - 1; i >= 0; i--)
		lmc_free(conns[i]);

	return ret == 0;
}

static int
op_test_1(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_2(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 30;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_3(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, LONG_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_4(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 20;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, LONG_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_5(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_6(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = lmc_flush(conn);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_7(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = test_log_lines(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
op_test_8(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

#ifdef __unix__
	signal(SIGPIPE, SIG_IGN);
#endif

	ret = lmc_disconnect(conn);

#ifdef __unix__
	signal(SIGPIPE, SIG_DFL);
#endif
	if (ret == 0) {
		printf("Should not be able to disconnect twice\n");
		ret = -1;
	} else {
		ret = 0;
	}

	lmc_free(conn);

	return ret == 0;
}

static int
op_test_9(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = lmc_unsubscribe(conn);
	if (ret != 0) {
		printf("Unsubscribe error\n");
		goto done;
	}

#ifdef __unix__
	signal(SIGPIPE, SIG_IGN);
#endif

	ret = lmc_disconnect(conn);

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
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_1(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_2(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 2;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_3(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 20;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_4(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_flush(conn);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_5(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	ret = lmc_flush(conn);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_6(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;
	uint64_t count, count_0, count_1;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = test_log_lines(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_7(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 2;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = test_log_lines(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_8(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 20;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = test_log_lines(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
combo_test_9(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = test_log_lines(conn);
	if (ret != 0)
		goto done;

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	count = 3;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = test_log_lines(conn);
	if (ret != 0)
		goto done;

	ret = lmc_flush(conn);
	if (ret != 0) {
		printf("Failed to flush data\n");
		goto done;
	}

	count = 3;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, SHORT_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != count) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = print_stats(conn);
	if (ret != 0)
		goto done;

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
incom_test_1(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(conn, LMC_ADD);
	lmc_free(conn);

	return ret == 0;
}

static int
incom_test_2(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(conn, LMC_STAT);
	lmc_free(conn);

	return ret == 0;
}

static int
incom_test_3(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(conn, LMC_FLUSH);
	lmc_free(conn);

	return ret == 0;
}

static int
incom_test_4(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(conn, LMC_GETLOGS);
	lmc_free(conn);

	return ret == 0;
}

static int
incom_test_5(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(conn, LMC_DISCONNECT);
	lmc_free(conn);

	return ret == 0;
}

static int
incom_test_6(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	ret = do_incomplete_op(conn, LMC_UNSUBSCRIBE);
	lmc_free(conn);

	return ret == 0;
}

static int
inv_test_1(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, LONG_LINE_LEN, 0);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != 0) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}

static int
inv_test_2(const void *param)
{
	struct lmc_conn *conn;
	const struct test_desc *test = param;
	uint64_t count, count_0, count_1;
	int ret;

	conn = lmc_connect((char *)test->tag);
	if (!conn) {
		printf("Client not connected\n");
		return 0;
	}

	count = 1;
	count_0 = count_log_lines(conn);

	ret = add_log_lines(conn, count, TOO_BIG_LINE_LEN, 1);
	if (ret != 0) {
		printf("Failed to add line\n");
		goto done;
	}

	count_1 = count_log_lines(conn);

	if (count_1 - count_0 != 0) {
		ret = -1;
		printf("Incorrect number of new logs\n");
		goto done;
	}

	ret = lmc_disconnect(conn);
	if (ret != 0)
		printf("Disconnect error\n");

done:
	lmc_free(conn);

	return ret == 0;
}


static const struct test_desc test_descs[] = {
	{ basic_test_1,	"basic-1",	"Client can connect to server" },
	{ basic_test_2,	"basic-2",	"Repeating connects from same client" },
	{ basic_test_3,	"basic-3",	"Connect and disconnect" },
	{ basic_test_4,	"basic-4",	"Repeating connects and disconnects" },

	{ op_test_1,	"op-1",		"Connect, add, disconnect" },
	{ op_test_2,	"op-2",		"Connect, many adds, disconnect" },
	{ op_test_3,	"op-3",		"Connect, add long line, disconnect" },
	{ op_test_4,	"op-4",		"Connect, add long lines, disconnect" },
	{ op_test_5,	"op-5",		"Connect, stat, disconnect" },
	{ op_test_6,	"op-6",		"Connect, flush, disconnect" },
	{ op_test_7,	"op-7",		"Connect, getlogs, disconnect" },
	{ op_test_8,	"op-8",		"Connect, disconnect, disconnect" },
	{ op_test_9,	"op-9",		"Connect, unsubscribe, disconnect" },

	{ combo_test_1,	"combo-1",	"Connect, add, stat, disconnect" },
	{ combo_test_2,	"combo-2",	"Connect, 2x add, stat, disconnect" },
	{ combo_test_3,	"combo-3",	"Connect, many adds, stat, disconnect" },
	{ combo_test_4,	"combo-4",	"Connect, add, flush, disconnect" },
	{ combo_test_5,	"combo-5",	"Connect, add, stat, flush, disconnect" },
	{ combo_test_6,	"combo-6",	"Connect, add, getlogs, disconnect" },
	{ combo_test_7,	"combo-7",	"Connect, 2x adds, getlogs, disconnect" },
	{ combo_test_8,	"combo-8",	"Connect, many adds, getlogs, disconnect" },
	{ combo_test_9,	"combo-9",	"Many ops" },

	{ incom_test_1,	"incom-1",	"Connect, incomplete add comm" },
	{ incom_test_2,	"incom-2",	"Connect, incomplete stat comm" },
	{ incom_test_3,	"incom-3",	"Connect, incomplete flush comm" },
	{ incom_test_4,	"incom-4",	"Connect, incomplete getlogs comm" },
	{ incom_test_5,	"incom-5",	"Connect, incomplete disconnect comm" },
	{ incom_test_6,	"incom-6",	"Connect, incomplete unsubscribe comm" },

	{ inv_test_1,	"inv-1",	"Connect, add line with non-printable characters" },
	{ inv_test_2,	"inv-2",	"Connect, add line too big, disconnect" },
};


static void
run_test(const struct test_desc *test)
{
	int success;
	char *msg;
	char padding[PADLEN];
	int len;

	memset(padding, '=', sizeof(padding));
	padding[sizeof(padding) - 1] = '\0';

	printf("[\x1b[0;33mTest %s: %s\x1b[0m]\n", test->tag, test->desc);

	sent_log_idx = 0;
	success = test->cb(test);
	msg = success ? "\x1b[0;32mSUCCESS\x1b[0m" : "\x1b[0;31mFAIL\x1b[0m";

	len = sizeof(padding) - (int)strlen(msg) - 1;
	printf("%s %.*s\n", msg, len, padding);
}

int
main(int argc, char *argv[])
{
	int i, rc;
	const char *tag;
	const struct test_desc *test;

	setbuf(stdout, NULL);
	srand((unsigned int) time(NULL));

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
