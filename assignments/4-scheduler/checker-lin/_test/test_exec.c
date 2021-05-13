/*
 * Threads scheduler execution tests
 *
 * 2017, Operating Systems
 */

#include "scheduler_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 12) Test exec no preemption
 *
 * tests if the scheduler properly runs exec without preemption
 */
static unsigned int test_exec_status;
static unsigned int test_exec_nr;
static tid_t test_exec_tid;

static void test_sched_handler_12(unsigned int dummy)
{
	unsigned int i;

	test_exec_tid = get_tid();

	for (i = 0; i < test_exec_nr; i++) {
		if (!this_tid(test_exec_tid))
			so_fail("other task id");
		so_exec();
	}
	test_exec_status = SO_TEST_SUCCESS;
}

void test_sched_12(void)
{
	unsigned int q;

	tid_t my_tid;

	test_exec_status = SO_TEST_FAIL;
	test_exec_nr = get_rand(2, SO_MAX_UNITS);

	/* choose a lower quantum than the number of execs */
	q = test_exec_nr / 2;

	so_init(q, 0);

	my_tid = so_fork(test_sched_handler_12, 0);

	so_end();

	basic_test(equal_tids(my_tid, test_exec_tid) && test_exec_status);
}

/*
 * 13) Test exec preemption
 *
 * tests if the scheduler properly preempts a task
 */
static tid_t test_exec_last_tid;
static tid_t test_tid_13_1;
static tid_t test_tid_13_2;

#define SO_TEST_AND_SET(expect_id, new_id) \
	do { \
		if (equal_tids((expect_id), INVALID_TID) || \
				equal_tids((new_id), INVALID_TID)) \
			so_fail("invalid task id"); \
		if (!equal_tids(test_exec_last_tid, (expect_id))) \
			so_fail("invalid tasks order"); \
		test_exec_last_tid = (new_id); \
	} while (0)

static void test_sched_handler_13_2(unsigned int dummy)
{
	SO_TEST_AND_SET(test_tid_13_1, test_tid_13_2);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_2, test_tid_13_2);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_1, test_tid_13_2);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_2, test_tid_13_2);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_1, test_tid_13_2);
	so_exec();
	test_exec_status = SO_TEST_SUCCESS;
}

static void test_sched_handler_13_1(unsigned int dummy)
{
	test_exec_last_tid = test_tid_13_1 = get_tid();
	test_tid_13_2 = so_fork(test_sched_handler_13_2, 0);

	/* allow the other thread to init */
	sched_yield();

	/* I should continue running */
	SO_TEST_AND_SET(test_tid_13_1, test_tid_13_1);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_2, test_tid_13_1);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_1, test_tid_13_1);
	so_exec();
	SO_TEST_AND_SET(test_tid_13_2, test_tid_13_1);
	so_exec();

	/* make sure nobody changed it until now */
	test_exec_status = SO_TEST_FAIL;
}

void test_sched_13(void)
{
	test_exec_status = SO_TEST_FAIL;

	/* quantum is 2, so each task should be preempted
	 * after running two instructions
	 */
	so_init(2, 0);

	so_fork(test_sched_handler_13_1, 0);

	sched_yield();
	so_end();

	basic_test(test_exec_status);
}

/*
 * 14) Test exec multiple
 *
 * tests if the scheduler properly preempts a task after a different one ends
 */
static tid_t test_tid_14_1;
static tid_t test_tid_14_2;
static tid_t test_tid_14_3;
static tid_t test_tid_14_4;

static void test_sched_handler_14_4(unsigned int dummy)
{
	SO_TEST_AND_SET(test_tid_14_3, test_tid_14_4);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_4, test_tid_14_4);

	test_exec_status = SO_TEST_FAIL;
}

static void test_sched_handler_14_3(unsigned int dummy)
{
	SO_TEST_AND_SET(test_tid_14_2, test_tid_14_3);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_3, test_tid_14_3);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_3, test_tid_14_3);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_3, test_tid_14_3);
	so_exec();
	/* should be preempted here */
	SO_TEST_AND_SET(test_tid_14_1, test_tid_14_3);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_3, test_tid_14_3);

	/* done scheduling */
	test_exec_status = SO_TEST_SUCCESS;
}

static void test_sched_handler_14_2(unsigned int dummy)
{
	SO_TEST_AND_SET(test_tid_14_1, test_tid_14_2);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_2, test_tid_14_2);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_2, test_tid_14_2);

	/* leaving - thread 3 should start */
	test_exec_status = SO_TEST_FAIL;
}

static void test_sched_handler_14_1(unsigned int dummy)
{
	test_exec_last_tid = test_tid_14_1 = get_tid();
	test_tid_14_2 = so_fork(test_sched_handler_14_2, 0);
	/* allow the other thread to init */
	sched_yield();

	/* I should continue running */
	SO_TEST_AND_SET(test_tid_14_1, test_tid_14_1);
	test_tid_14_3 = so_fork(test_sched_handler_14_3, 0);
	sched_yield();

	/* I should continue running */
	SO_TEST_AND_SET(test_tid_14_1, test_tid_14_1);
	test_tid_14_4 = so_fork(test_sched_handler_14_4, 0);
	sched_yield();

	/* still me */
	SO_TEST_AND_SET(test_tid_14_1, test_tid_14_1);
	so_exec();

	/* should be preempted here */
	SO_TEST_AND_SET(test_tid_14_4, test_tid_14_1);
	so_exec();
	SO_TEST_AND_SET(test_tid_14_1, test_tid_14_1);

	/* leaving - make sure the others execute successfully */
	test_exec_status = SO_TEST_FAIL;
}

void test_sched_14(void)
{
	test_exec_status = SO_TEST_FAIL;

	so_init(4, 0);

	so_fork(test_sched_handler_14_1, 0);

	sched_yield();
	so_end();

	basic_test(test_exec_status);
}

#undef SO_TEST_AND_SET

/*
 * 15) Test exec priority
 *
 * tests if the scheduler properly preempts a task according to priorities
 */
static unsigned int test_exec_last_priority;

/* fails if the last priority set is not prio */
#define SO_FAIL_IF_NOT_PRIO(prio, msg) \
	do { \
		if ((prio) != test_exec_last_priority) { \
			test_exec_status = SO_TEST_FAIL; \
			so_fail(msg); \
		} \
		test_exec_last_priority = priority; \
	} while (0)

/*
 * Note: P1 means tasks with priority 1
 *
 * Scenario:
 * - P2 spawns P4
 * - P4 spawns P3
 * - P3 spawns P1
 */
static void test_sched_handler_15(unsigned int priority)
{
	switch (priority) {
	case 1:
		/* test if I was scheduled before P2 */
		SO_FAIL_IF_NOT_PRIO(2,
			"scheduled a test with a bogus priority");
		break;

	case 2:
		/* fork P4 */
		so_fork(test_sched_handler_15, 4);

		/* if I was not preempted or P3 didn't run - error */
		SO_FAIL_IF_NOT_PRIO(3, "task 2 was not preempted");
		break;

	case 3:
		/* test if someobdy else run except P4 */
		SO_FAIL_IF_NOT_PRIO(4, "highest priority was not scheduled");
		so_fork(test_sched_handler_15, 1);

		/* P1 < P3 - I still have to run */
		SO_FAIL_IF_NOT_PRIO(3,
			"somebody else was scheduled instead of task 3");
		break;

	case 4:
		test_exec_last_priority = 4;

		/* fork lower priority P3 */
		so_fork(test_sched_handler_15, 3);

		/* I shouldn't have been preempted */
		SO_FAIL_IF_NOT_PRIO(4,
			"somebody else was scheduled instead of task 4");
		break;
	}

	so_exec();
}

void test_sched_15(void)
{
	test_exec_status = SO_TEST_SUCCESS;
	so_init(1, 0);

	test_exec_last_priority = 2;
	so_fork(test_sched_handler_15, 2);

	sched_yield();
	so_end();

	basic_test(test_exec_status && test_exec_last_priority == 1);
}

#undef SO_FAIL_IF_NOT_PRIO

/*
 * Structures and variables needed for the next tests
 */

struct so_task_info_t {
	unsigned int creation_time;
	unsigned int priority;
	unsigned int executed;
	unsigned int runtime;
	tid_t tid;
};

/* maximum iterations of the algorithm */
#define SO_MAX_EXECUTION_TIME (SO_MAX_UNITS * SO_MAX_UNITS)

/*
 * again, these variables should be guarded by the scheduler itself,
 * because only one function can be executed at a certain moment
 * test_16, test_17 and test_18 are using the variables, they should
 * be reinitialized in the test function
 */
static unsigned int quantum;
static unsigned int exec_time;
static unsigned int tasks_no;
static struct so_task_info_t tasks_info[SO_MAX_EXECUTION_TIME];
static struct so_task_info_t *tasks_history[SO_MAX_EXECUTION_TIME];

/* checks if there is a higher priority waiting to run */
static inline int is_higher(unsigned int *vec, unsigned int prio)
{
	for (prio++; prio < SO_MAX_PRIO; prio++)
		if (vec[prio] != 0)
			return 1;
	return 0;
}

/*
 * Tests if the tasks properly executed
 * ATTENTION: tasks_history and tasks_info should be populated
 */
static void test_sched_generic_check(void)
{
	unsigned int priority_stats[SO_MAX_PRIO];
	unsigned int idx;
	unsigned int total_exec_time = 0;
	struct so_task_info_t *last_task = 0;
	struct so_task_info_t *current_task;

	/* check to see if anything was executed */
	if (exec_time == 0)
		so_fail("nothing was executed");

	/* integrity check - all the tasks had been consumed */
	for (idx = 0; idx < tasks_no; idx++)
		total_exec_time += tasks_info[idx].executed;

	if (total_exec_time > SO_MAX_EXECUTION_TIME ||
			total_exec_time != exec_time ||
			(exec_time == SO_MAX_EXECUTION_TIME &&
			 test_exec_status != SO_TEST_SUCCESS)) {
		so_fail("total execution time mismatch");
	}

	test_exec_status = SO_TEST_FAIL;
	memset(priority_stats, 0, sizeof(priority_stats));

	/* for each time unit elapsed */
	for (idx = 0; idx < exec_time; idx++) {
		current_task = tasks_history[idx];

		/* if I wasn't the last task, then I was preempted */
		if (last_task != current_task) {
			/* if they have the same priority */
			if (last_task && last_task->executed != 0 &&
				current_task->creation_time != idx &&
				last_task->priority == current_task->priority &&
				last_task->runtime != quantum)
				so_fail("previous task did not complete it's "
					"quantum");
			last_task = current_task;
			current_task->runtime = 0;
		}
		/* check if it has nothing more to execute */
		if (current_task->executed == 0)
			so_fail("tasks has nothing more to execute");

		/* check if the task was created */
		if (idx == current_task->creation_time)
			priority_stats[current_task->priority]++;

		/* check if there is a task with higher priority waiting */
		if (is_higher(priority_stats, current_task->priority))
			so_fail("higher priority waiting in queue");

		current_task->executed--;
		current_task->runtime++;

		/* if nothing more to execute -> leave */
		if (current_task->executed == 0) {
			priority_stats[current_task->priority]--;
		} else if (priority_stats[current_task->priority] > 1 &&
				current_task->runtime > quantum) {
			so_fail("task was not preempted");
		}
	}
	test_exec_status = SO_TEST_SUCCESS;
}

static void test_sched_handler(unsigned int priority)
{
	int i;
	struct so_task_info_t *my_info;

	/* fill info about my task */
	my_info = &tasks_info[tasks_no++];
	my_info->creation_time = exec_time;
	my_info->priority = priority;
	my_info->tid = get_tid();
	my_info->executed = 0;

	my_info->executed++;
	tasks_history[exec_time++] = my_info;

	if (exec_time == 0) {
		/* If the first task spawn other 3 tasks */
		for (i = 0; i < 3; i++)
			so_fork(test_sched_handler, priority);
	}

	for (i = 0; i < SO_MAX_UNITS / 2; i++) {
	/* Run only half of the allowed quantum */
		my_info->executed++;
		tasks_history[exec_time++] = my_info;
		so_exec();
	}
}

/*
 * 16) Test round robin (exec_time < quantum)
 *
 * tests if the scheduler properly schedules the same priority task
 */
void test_sched_16(void)
{
	/*
	 * test_sched_{16,17,18} are using some common variables make
	 * sure we initialize them -- even thought they are static
	 */
	quantum = 0;
	exec_time = 0;
	tasks_no = 0;
	memset(tasks_info, 0, sizeof(tasks_info));
	memset(tasks_history, 0, sizeof(tasks_history));

	test_exec_status = SO_TEST_FAIL;

	/* Each task is allowed to run SO_MAX_UNITS
	 * before it gets preempted
	 */
	so_init(SO_MAX_UNITS, 0);

	/* spawn the first task */
	so_fork(test_sched_handler, get_rand(0, SO_MAX_PRIO));

	sched_yield();
	so_end();

	test_sched_generic_check();

	basic_test(test_exec_status);
}

/*
 * 17) Test round robin (exec_time > quantum)
 *
 * tests if the scheduler properly schedules the same priority task
 */
void test_sched_17(void)
{
	/*
	 * test_sched_{16,17,18} are using some common variables make
	 * sure we initialize them -- even thought they are static
	 */
	quantum = 0;
	exec_time = 0;
	tasks_no = 0;
	memset(tasks_info, 0, sizeof(tasks_info));
	memset(tasks_history, 0, sizeof(tasks_history));

	test_exec_status = SO_TEST_FAIL;

	so_init(SO_MAX_UNITS / 4, 0);

	/* spawn task */
	so_fork(test_sched_handler, get_rand(0, SO_MAX_PRIO));

	sched_yield();
	so_end();

	test_sched_generic_check();

	basic_test(test_exec_status);
}

/*
 * 18) Test round robin
 *
 * tests if the scheduler properly preempts a task according to priorities
 */
static void test_sched_handler_18(unsigned int priority)
{
	unsigned int executed_fork = 0;
	unsigned int rand_iterations;
	struct so_task_info_t *my_info;

	if (tasks_no >= SO_MAX_EXECUTION_TIME)
		return;

	/* fill info about my task */
	my_info = &tasks_info[tasks_no++];
	my_info->creation_time = exec_time;
	my_info->priority = priority;
	my_info->tid = get_tid();
	my_info->executed = 0;

	/* get a rand number of iterations to do */
	rand_iterations = get_rand(1, SO_MAX_UNITS);

	while (rand_iterations-- && exec_time < SO_MAX_EXECUTION_TIME) {

		/* fill in history */
		my_info->executed++;
		tasks_history[exec_time++] = my_info;

		/*
		 * here we force all tasks to execute at least one fork
		 * if it was executed previously, then we offer random
		 * chances to execute either fork or exec
		 */
		if ((!executed_fork && rand_iterations == 0) || rand() % 2) {
			/* create new task with random priority */
			so_fork(test_sched_handler_18,
				get_rand(0, SO_MAX_PRIO));
			executed_fork = 1;
		} else {
			so_exec();
		}
	}
	if (exec_time == SO_MAX_EXECUTION_TIME)
		test_exec_status = SO_TEST_SUCCESS;
}

void test_sched_18(void)
{
	/*
	 * test_sched_{16,17,18} are using some common variables make
	 * sure we initialize them -- even thought they are static
	 */
	quantum = 0;
	exec_time = 0;
	tasks_no = 0;
	memset(tasks_info, 0, sizeof(tasks_info));
	memset(tasks_history, 0, sizeof(tasks_history));

	test_exec_status = SO_TEST_FAIL;

	/* random execution runtime */
	quantum = get_rand(1, SO_MAX_UNITS / 2);

	so_init(quantum, 0);

	/* spawn task */
	so_fork(test_sched_handler_18, get_rand(0, SO_MAX_PRIO));

	sched_yield();
	so_end();

	test_sched_generic_check();

	basic_test(test_exec_status);
}
