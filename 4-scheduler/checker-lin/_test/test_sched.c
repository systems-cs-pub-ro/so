/*
 * Threads scheduler tests
 *
 * 2017, Operating Systems
 */

#include "scheduler_test.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * 1) Test init params
 *
 * tests if the init parameters are properly checked
 */
void test_sched_01(void)
{
	int integrity_test = SO_TEST_SUCCESS;

	if (so_init(0, 0) == 0) {
		so_error("invalid time quantum");
		integrity_test = SO_TEST_FAIL;
		goto test;
	}

	if (so_init(1, SO_MAX_NUM_EVENTS + 1) == 0) {
		so_error("invalid I/O devices");
		integrity_test = SO_TEST_FAIL;
		goto test;
	}
test:
	so_end();

	basic_test(integrity_test == SO_TEST_SUCCESS);
}

/*
 * 2) Test init
 *
 * tests if init is properly returns the correct value
 */
void test_sched_02(void)
{
	int ret = 0;

	ret = so_init(SO_MAX_UNITS, 0);
	if (ret < 0) {
		so_error("initialization failed");
		goto test;
	}

	/* clean the previous init */
	so_end();

	ret = so_init(SO_MAX_UNITS, SO_MAX_NUM_EVENTS);
	if (ret < 0) {
		so_error("initialization failed");
		goto test;
	}

test:
	so_end();

	basic_test(ret == 0);
}

/*
 * 3) Test double init
 *
 * tests if the scheduler can be initiated two times
 */
void test_sched_03(void)
{
	int ret = so_init(SO_MAX_UNITS, 0);

	if (ret < 0) {
		so_error("initialization failed");
		goto test;
	}

	if (so_init(SO_MAX_UNITS, 0) >= 0) {
		so_error("scheduler initialized two times");
		ret = -1;
	}

test:
	so_end();

	basic_test(ret == 0);
}

/*
 * 4) Test reinit
 *
 * tests if the scheduler can be initiated after deletion
 */
void test_sched_04(void)
{
	int ret = so_init(SO_MAX_UNITS, 0);

	if (ret < 0) {
		so_error("initialization failed");
		goto test;
	}

	/* clean the previous init */
	so_end();

	if (so_init(SO_MAX_UNITS, 0) != 0) {
		so_error("second scheduler init failed");
		ret = -1;
	}

test:
	so_end();

	basic_test(ret == 0);
}

/*
 * 5) Test end before init
 *
 * tests if it is safe to call end before init
 */
void test_sched_05(void)
{
	int ret = 0;

	so_end();

	ret = so_init(SO_MAX_UNITS, 0);
	if (ret < 0) {
		so_error("initialization failed");
		goto test;
	}

test:
	so_end();

	basic_test(ret == 0);
}

/*
 * 6) Test fork params
 *
 * tests scheduler fork parameters
 */
static void test_sched_handler_06(unsigned int prio) {}

void test_sched_06(void)
{
	int integrity_test = SO_TEST_FAIL;

	if (so_init(SO_MAX_UNITS, 0) < 0) {
		so_error("initialization failed");
		goto test;
	}

	/* invalid handler */
	if (so_fork(0, 0) != INVALID_TID) {
		so_error("invalid handler");
		goto test;
	}

	/* invalid prio */
	if (so_fork(test_sched_handler_06, SO_MAX_PRIO + 1) != INVALID_TID) {
		so_error("invalid prio");
		goto test;
	}

	integrity_test = SO_TEST_SUCCESS;

test:
	so_end();

	basic_test(integrity_test == SO_TEST_SUCCESS);
}

/*
 * 7) Test fork handler
 *
 * tests if the scheduler runs the handler
 */
static unsigned int test_sched_fork_executed;

static void test_sched_handler_07(unsigned int prio)
{
	test_sched_fork_executed = 1;
}

void test_sched_07(void)
{
	test_sched_fork_executed = 0;

	if (so_init(SO_MAX_UNITS, 0) < 0) {
		so_error("initialization failed");
		goto test;
	}

	/* invalid handler */
	if (so_fork(test_sched_handler_07, 0) == INVALID_TID) {
		so_error("fork failed");
		goto test;
	}

test:
	so_end();

	basic_test(test_sched_fork_executed != 0);
}

/*
 * 8) Test fork priority
 *
 * tests if the scheduler runs the handler
 */
static unsigned int test_sched_fork_priority;

static void test_sched_handler_08(unsigned int prio)
{
	test_sched_fork_priority = prio;
}

void test_sched_08(void)
{
	unsigned int prio = get_rand(1, SO_MAX_PRIO);

	test_sched_fork_priority = 0;

	if (so_init(SO_MAX_UNITS, 0) < 0) {
		so_error("initialization failed");
		goto test;
	}

	if (so_fork(test_sched_handler_08, prio) == INVALID_TID) {
		so_error("fork failed");
		goto test;
	}

test:
	so_end();

	basic_test(test_sched_fork_priority == prio);
}

/*
 * 9) Test fork thread id
 *
 * tests if fork creates a new thread
 */
static tid_t test_fork_tid;

static void test_sched_handler_09(unsigned int prio)
{
	test_fork_tid = get_tid();
}

void test_sched_09(void)
{
	tid_t new_tid;
	tid_t local_tid = get_tid();

	test_fork_tid = INVALID_TID;

	if (so_init(SO_MAX_UNITS, 0) < 0) {
		so_error("initialization failed");
		goto test;
	}

	new_tid = so_fork(test_sched_handler_09, 0);

test:
	so_end();

	basic_test(!equal_tids(new_tid, INVALID_TID) &&
				!equal_tids(test_fork_tid, INVALID_TID) &&
				!equal_tids(new_tid, local_tid) &&
				!this_tid(new_tid) &&
				equal_tids(new_tid, test_fork_tid));
}

/*
 * 10) Test multiple fork
 *
 * tests if the scheduler runs each fork
 */
static unsigned int test_sched_fork_handler_runs;
static unsigned int test_sched_fork_runs;

static void test_sched_handler_10_worker(unsigned int prio)
{
	test_sched_fork_handler_runs++;
}

static void test_sched_handler_10_master(unsigned int prio)
{
	unsigned int run;

	for (run = 0; run < test_sched_fork_runs; run++)
		so_fork(test_sched_handler_10_worker, 0);
}

void test_sched_10(void)
{
	test_sched_fork_runs = get_rand(1, SO_MAX_UNITS);

	if (so_init(SO_MAX_UNITS, 0) < 0) {
		so_error("initialization failed");
		goto test;
	}

	so_fork(test_sched_handler_10_master, SO_MAX_PRIO);
	sched_yield();

test:
	so_end();

	basic_test(test_sched_fork_handler_runs == test_sched_fork_runs);
}

/*
 * 11) Test multiple fork thread ids
 *
 * tests if the scheduler runs each fork with a different id
 */
static unsigned int test_fork_rand_tests;
static unsigned int test_fork_execution_status;
static tid_t test_fork_exec_tids[SO_MAX_UNITS];
static tid_t test_fork_tids[SO_MAX_UNITS];

static void test_sched_handler_11_worker(unsigned int dummy)
{
	static unsigned int exec_idx;

	/* signal that he's the one that executes in round exec_idx */
	test_fork_exec_tids[exec_idx++] = get_tid();
	test_fork_execution_status = SO_TEST_SUCCESS;
}

static void test_sched_handler_11_master(unsigned int dummy)
{
	unsigned int i;

	/*
	 * this thread should not be preempted as it executes maximum
	 * SO_MAX_UNITS - 1, and the quantum time is SO_MAX_UNITS
	 */
	/* use a cannary value to detect overflow */
	test_fork_tids[test_fork_rand_tests] = get_tid();
	test_fork_exec_tids[test_fork_rand_tests] = get_tid();
	for (i = 0; i < test_fork_rand_tests; i++)
		test_fork_tids[i] = so_fork(test_sched_handler_11_worker, 0);
}

void test_sched_11(void)
{
	unsigned int i;

	test_fork_rand_tests = get_rand(1, SO_MAX_UNITS - 1);
	test_fork_execution_status = SO_TEST_FAIL;

	so_init(SO_MAX_UNITS, 0);

	so_fork(test_sched_handler_11_master, 0);

	sched_yield();
	so_end();

	if (test_fork_execution_status == SO_TEST_SUCCESS) {
		/* check threads order */
		for (i = 0; i <= test_fork_rand_tests; i++) {
			if (!equal_tids(test_fork_exec_tids[i],
				test_fork_tids[i])) {
				so_error("different thread ids");
				test_fork_execution_status = SO_TEST_FAIL;
				break;
			}
		}
	} else {
		so_error("threads execution failed");
	}

	basic_test(test_fork_execution_status);
}

