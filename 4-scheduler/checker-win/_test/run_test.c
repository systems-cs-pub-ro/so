/*
 * Testing framework
 *
 * 2012, Operating Systems
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

/* tests functions */
#include "run_test.h"
/* custom tests */
#include "scheduler_test.h"

#define MAX_POINTS 95

/* global variables used by the test */
static char *description;
static int points;
static unsigned long test_index;

/* prints the header specified aligned with the other output */
static void print_header(const char *header)
{
	int i, padding;
	int header_len = strlen(header);

	printf("\n");
	if (header_len < 71) {
		padding = (71 - header_len) / 2;
		for (i = 0; i < padding; i++)
			printf(" ");
	}
	printf("= %s =\n\n", header);
}

void test_do_fail(void)
{
	printf("failed  [ 0/%02d]\n", MAX_POINTS);
#ifdef EXIT_IF_FAIL
	exit(EXIT_FAILURE);
#endif
}

void test_do_pass(void)
{
	printf("passed  [%02d/%02d]\n", points, MAX_POINTS);
}

void basic_test(int true)
{
	int i;
	int desc_len = strlen(description);

	printf("%02lu) %s", test_index + 1, description);
	for (i = 0; i < 56 - desc_len; i++)
		printf(".");
	if (true)
		test_do_pass();
	else
		test_do_fail();
}

static int init_world(void)
{
	print_header("Testing - threads scheduler");
	return 0;
}

static int cleanup_world(void)
{
	return 0;
}

struct run_test_t test_fun_array[] = {
	/* tests schedule - see test_sched.c */
	{ test_sched_01,	"Test init params",			1  },
	{ test_sched_02,	"Test init",				1  },
	{ test_sched_03,	"Test double init",			1  },
	{ test_sched_04,	"Test reinit",				1  },
	{ test_sched_05,	"Test end before init",			1  },
	{ test_sched_06,	"Test fork params",			1  },
	{ test_sched_07,	"Test fork handler",			2  },
	{ test_sched_08,	"Test fork priority",			2  },
	{ test_sched_09,	"Test fork thread id",			2  },
	{ test_sched_10,	"Test multiple fork",			3  },
	{ test_sched_11,	"Test multiple fork thread ids",	3  },

	/* tests schedule execution - see test_exec.c */
	{ test_sched_12,	"Test exec no preemption",		3  },
	{ test_sched_13,	"Test exec preemption",			4  },
	{ test_sched_14,	"Test exec multiple",			6  },
	{ test_sched_15,	"Test exec priorities",			8  },
	{ test_sched_16,	"Test round robin",			20 },

	/* tests schedule IO operations - see test_io.c */
	{ test_sched_17,	"Test IO devices",			1  },
	{ test_sched_18,	"Test IO schedule",			8  },
	{ test_sched_19,	"Test priorities and IO",		12 },
	{ test_sched_20,	"Test priorities and IO (stress test)",	10 },
};

/* custom main testing thread */
int main(int argc, char **argv)
{
	unsigned long last_test;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <test_number> | init | cleanup\n",
			argv[0]);
		return -1;
	}

	/* check init and cleanup execution */
	if (strcmp(argv[1], "init") == 0) {
		if (init_world() < 0) {
			fprintf(stderr, "test init failed\n");
			return -1;
		}
		return 0;
	} else if (strcmp(argv[1], "cleanup") == 0) {
		if (cleanup_world() < 0) {
			fprintf(stderr, "test cleanup failed\n");
			return -1;
		}
		return 0;
	}

	test_index = strtoul(argv[1], NULL, 10) - 1;
	if (errno == EINVAL || errno == ERANGE) {
		fprintf(stderr, "%s not a number\n", argv[1]);
		return -1;
	}

	last_test = sizeof(test_fun_array) / sizeof(struct run_test_t) - 1;
	if (test_index < 0 || test_index > last_test) {
		fprintf(stderr, "Error: Test index is out of range"
			"(1 < test_index <= %lu).\n", last_test + 1);
		return -1;
	}

	/* randomize time quantums */
	srand((unsigned long)time(NULL));

	description = test_fun_array[test_index].description;
	points = test_fun_array[test_index].points;
	test_fun_array[test_index].function();

	return 0;
}

