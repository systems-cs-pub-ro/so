#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lmc.h"


int main() {
	struct lmc_conn *conn;

	conn = lmc_connect(NULL);

	printf("conn = %p\n", conn);

	if (conn != NULL)
		fprintf(stderr, "Connection was created\n");
	else
		fprintf(stderr, "Connection was not created\n");

	lmc_send_log(conn, "this is a log line");

	lmc_disconnect(conn);

	return 0;
}
