#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lmc.h"


int main() {
	struct lmc_conn *conn;

	conn = lmc_connect(NULL);
	lmc_unsubscribe(conn);

	printf("conn = %p\n", conn);

	return 0;
}
