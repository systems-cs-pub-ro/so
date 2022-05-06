#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lmc.h"

int main() {
	struct lmc_conn *conn;
	char *stats;

	conn = lmc_connect(NULL);

	lmc_send_log(conn, "this is a log line");
	lmc_send_log(conn, "this is a log line");
	lmc_send_log(conn, "this is a log line");
	lmc_send_log(conn, "this is a log line");

	stats = lmc_get_stats(conn);
	printf("%s\n", stats);

	return 0;
}
