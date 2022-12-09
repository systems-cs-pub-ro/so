#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lmc.h"

int main() {
	char longline[LMC_LOGLINE_SIZE + 1];
	struct lmc_conn *conn;
	struct lmc_client_logline **lines;
	char *stats;
	uint64_t logs, i;

	conn = lmc_connect(NULL);

	for (i = 0; i < LMC_LOGLINE_SIZE; i++)
		longline[i] = 'a';
	longline[LMC_LOGLINE_SIZE] = 0;
	lmc_send_log(conn, longline);

	for (i = 0; i < LMC_LOGLINE_SIZE; i++)
		longline[i] = 'b';
	longline[LMC_LOGLINE_SIZE] = 0;
	lmc_send_log(conn, longline);

	for (i = 0; i < LMC_LOGLINE_SIZE; i++)
		longline[i] = 'c';
	longline[LMC_LOGLINE_SIZE] = 0;
	lmc_send_log(conn, longline);

	stats = lmc_get_stats(conn);
	printf("%s\n", stats);
	lines = lmc_get_logs(conn, 0, 0, &logs);

	for (i = 0; i < logs; i++) {
		printf("%s: %s\n", lines[i]->time, lines[i]->logline);
	}

	return 0;
}
