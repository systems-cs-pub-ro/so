#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lmc.h"

char logline[200];

int main() {
	int rc;
	struct lmc_conn *conn;

	conn = lmc_connect("specialC");

	printf("conn = %p\n", conn);

	for (rc = 0; rc < 50; rc ++) {
		fprintf(stderr, "Send log no %d\n", rc);
		memset(logline, 0, 200);
		sprintf(logline, "this is logline no %d",rc);
		lmc_send_log(conn, logline);
	}

	lmc_flush(conn);

	return 0;
}
