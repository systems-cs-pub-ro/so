/*
 * Threads scheduler io tests
 *
 * 2017, Operating Systems
 */

#include "scheduler_test.h"

#include <stdio.h>
#include <stdlib.h>

#define SO_DEV0		0
#define SO_DEV1		1
#define SO_DEV2		2
#define SO_DEV3		3

#define SO_PREEMPT_UNITS	3

static unsigned int exec_time;
static unsigned int exec_devs;
static unsigned int last_priority;
static unsigned int exec_priority;
static unsigned int test_exec_status = SO_TEST_FAIL;

/*
 * 19) Test IO devices
 *
 * tests if the scheduler properly uses the IO devices
 */
static void test_sched_handler_19(unsigned int prio)
{
	if (prio != exec_priority)
		so_fail("invalid exec priority");

	if (so_wait(exec_devs) == 0)
		so_fail("invalid waiting device");

	if (so_signal(exec_devs) >= 0)
		so_fail("invalid signalling device");

	if (so_signal(SO_DEV0) != 0)
		so_fail("too many threads woken");

	test_exec_status = SO_TEST_SUCCESS;
}

void test_sched_19(void)
{
	test_exec_status = SO_TEST_FAIL;
	exec_devs = get_rand(1, SO_MAX_UNITS);
	exec_priority = get_rand(1, SO_MAX_PRIO);

	if (so_init(SO_MAX_UNITS, exec_devs) < 0) {
		so_error("initialization failed");
		goto test;
	}

	if (so_fork(test_sched_handler_19, exec_priority) == INVALID_TID) {
		so_error("cannot create new task");
		goto test;
	}

test:
	sched_yield();
	so_end();

	basic_test(test_exec_status);
}

/*
 * 20) Test IO schedule
 *
 * tests if the scheduler properly handles IO devices
 */
static void test_sched_handler_20_signal(unsigned int dummy)
{
	unsigned int step;

	/* check if wait was called */
	if (exec_time != 2) {
		so_error("thread didn't execute wait");
		return;
	}

	/* keep the other thread waiting longer than the preemption time */
	for (step = 0; step <= SO_PREEMPT_UNITS; step++) {
		exec_time++;
		so_exec();
	}

	/* if executed before signal, fail */
	if (test_exec_status == SO_TEST_SUCCESS)
		test_exec_status = SO_TEST_FAIL;
	/* finally release the waiting thread */
	so_signal(SO_DEV0);
}

static void test_sched_handler_20_wait(unsigned int dummy)
{
	exec_time++;
	so_fork(test_sched_handler_20_signal, 0);
	exec_time++;
	so_wait(SO_DEV0);

	/* check if I waited more than preemption time */
	if (exec_time < SO_PREEMPT_UNITS + 2) {
		so_error("scheduled while waiting");
		return;
	}

	so_exec();
	test_exec_status = SO_TEST_SUCCESS;
}

/* tests the IO functionality */
void test_sched_20(void)
{
	/* ensure that the thread gets to execute wait */
	so_init(SO_PREEMPT_UNITS, 1);

	so_fork(test_sched_handler_20_wait, 0);

	sched_yield();
	so_end();

	basic_test(test_exec_status);
}

/*
 * 21) Test priorities and IO
 *
 * tests if the scheduler properly handles IO devices and preemption
 */
/* fails if the last priority set is not prio */
#define FAIL_IF_NOT_PRIO(prio, msg) \
	do { \
		if ((prio) != last_priority) { \
			test_exec_status = SO_TEST_FAIL; \
			so_fail(msg); \
		} \
		last_priority = priority; \
	} while (0)

/*
 * Threads are mixed to wait/signal lower/higher priorities
 * P2 refers to the task with priority 2
 */
static void test_sched_handler_21(unsigned int priority)
{
	switch (priority) {
	case 1:
		/* P2 should be the one that executed last */
		FAIL_IF_NOT_PRIO(2, "should have been woke by P2");
		if (so_signal(SO_DEV3) == 0)
			so_fail("dev3 does not exist");
		so_exec();
		FAIL_IF_NOT_PRIO(1, "invalid preemption");
		if (so_signal(SO_DEV0) != 2)
			so_fail("P1 should wake P3 and P4 (dev0)");
		FAIL_IF_NOT_PRIO(2, "preempted too early");
		if (so_signal(SO_DEV1) != 1)
			so_fail("P1 should wake P3 (dev1)");
		FAIL_IF_NOT_PRIO(2, "woke by someone else");
		if (so_signal(SO_DEV0) != 1)
			so_fail("P1 should wake P4 (dev0)");
		FAIL_IF_NOT_PRIO(4, "should be the last one");
		so_exec();
		FAIL_IF_NOT_PRIO(1, "someone else was running");
		break;

	case 2:
		last_priority = 2;
		/* wait for dev 3 - invalid device */
		if (so_wait(SO_DEV3) == 0)
			so_fail("dev3 does not exist");
		/* spawn all the tasks */
		so_fork(test_sched_handler_21, 4);
		so_fork(test_sched_handler_21, 3);
		so_fork(test_sched_handler_21, 1);
		so_exec();
		so_exec();

		/* no one should have ran until now */
		FAIL_IF_NOT_PRIO(2, "somebody else ran before P2");
		if (so_wait(SO_DEV1) != 0)
			so_fail("cannot wait on dev1");
		FAIL_IF_NOT_PRIO(3, "should run after P3");
		if (so_wait(SO_DEV2) != 0)
			so_fail("cannot wait on dev2");
		FAIL_IF_NOT_PRIO(3, "only P3 could wake me");
		so_exec();
		break;

	case 3:
		if (so_wait(SO_DEV0) != 0)
			so_fail("P3 cannot wait on dev0");
		FAIL_IF_NOT_PRIO(4, "priority order violated");
		if (so_wait(SO_DEV1) != 0)
			so_fail("P3 cannot wait on dev1");
		FAIL_IF_NOT_PRIO(1, "someone else woke P3");
		if (so_signal(SO_DEV2) != 1)
			so_fail("P3 should wake P2 (dev2)");
		break;

	case 4:
		if (so_wait(SO_DEV0) != 0)
			so_fail("P4 cannot wait on dev0");
		FAIL_IF_NOT_PRIO(1, "lower priority violation");
		if (so_signal(SO_DEV1) != 1)
			so_fail("P4 should wake P2 (dev1)");
		if (so_wait(SO_DEV0) != 0)
			so_fail("P4 cannot wait on dev0");
		FAIL_IF_NOT_PRIO(1, "someone else woke dev0");
		break;
	}
}

/* tests the IO and priorities */
void test_sched_21(void)
{
	test_exec_status = SO_TEST_SUCCESS;

	so_init(1, 3);

	so_fork(test_sched_handler_21, 2);

	sched_yield();
	so_end();

	basic_test(test_exec_status == SO_TEST_SUCCESS && last_priority == 1);
}

#undef FAIL_IF_NOT_PRIO

/*
 * 22) Test priorities and IO (stress test)
 *
 * tests if the scheduler properly handles IO devices and preemption
 */

static unsigned int num_executions;

/**
 * high priority thread handler
 */
static void test_sched_handler_22_high_worker(unsigned int priority)
{
	static unsigned int high_dev;

	so_exec();

	/* reserve a device */
	++high_dev;

	/* high priority threads will run first
	 * and will wait on devices
	 */
	if (so_wait(high_dev))
		so_fail("cannot wait on dev");

	so_exec();

	/* wake the master thread */
	if (so_signal(0) != 1)
		so_fail("should wake only one thread");

	so_exec();
}

/**
 * medium priority thread handler
 */
static void test_sched_handler_22_med_worker(unsigned int priority)
{
	static unsigned int med_dev1;
	static unsigned int med_dev2 = SO_MAX_NUM_EVENTS;

	so_exec();

	/* reserve a device */
	++med_dev1;

	/* medium priority threads will run second
	 * and will wake up high priority threads
	 */
	if (so_signal(med_dev1) != 1)
		so_fail("should wake only one thread");

	so_exec();

	/* reserve a device */
	--med_dev2;

	/* medium priority threads will wait on devices */
	if (so_wait(med_dev2))
		so_fail("cannot wait on dev");

	so_exec();
}

/**
 * low priority thread handler
 */
static void test_sched_handler_22_low_worker(unsigned int priority)
{
	static unsigned int low_dev = SO_MAX_NUM_EVENTS;

	so_exec();

	/* reserve a device */
	--low_dev;

	/* low priority threads will run third
	 * and will wake up medium priority threads
	 */
	if (so_signal(low_dev) != 1)
		so_fail("should wake only one thread");

	so_exec();

	++num_executions;
}

/**
 * master thread handler
 */
static void test_sched_handler_22_master(unsigned int priority)
{
	int err;
	int i;
	unsigned int high_prio = SO_MAX_PRIO - 1;
	unsigned int med_prio = SO_MAX_PRIO - 2;
	unsigned int low_prio = SO_MAX_PRIO - 3;

	/* create SO_MAX_NUM_EVENTS/2 threads with high priority */
	for (i = 0; i < SO_MAX_NUM_EVENTS; i += 2) {
		err = so_fork(test_sched_handler_22_high_worker, high_prio);
		if (err == INVALID_TID)
			so_fail("cannot create new task");
	}

	/* create SO_MAX_NUM_EVENTS/2 threads with medium priority */
	for (i = 0; i < SO_MAX_NUM_EVENTS; i += 2) {
		err = so_fork(test_sched_handler_22_med_worker, med_prio);
		if (err == INVALID_TID)
			so_fail("cannot create new task");
	}

	/* wait on dev 0 for SO_MAX_NUM_EVENTS/2 times */
	for (i = 0; i < SO_MAX_NUM_EVENTS; i += 2)
		if (so_wait(0))
			so_fail("cannot wait on dev");

	/* create SO_MAX_NUM_EVENTS/2 threads with low priority */
	for (i = 0; i < SO_MAX_NUM_EVENTS; i += 2) {
		err = so_fork(test_sched_handler_22_low_worker, low_prio);
		if (err == INVALID_TID)
			so_fail("cannot create new task");
	}
}

/* tests the IO and priorities */
void test_sched_22(void)
{
	so_init(1, SO_MAX_NUM_EVENTS);

	so_fork(test_sched_handler_22_master, SO_MAX_PRIO);

	sched_yield();
	so_end();

	basic_test(num_executions == SO_MAX_NUM_EVENTS / 2);
}
