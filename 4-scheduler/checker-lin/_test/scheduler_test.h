/*
 * Threads scheduler functions
 *
 * 2012, Operating Systems
 */

#ifndef SCHED_TEST_H_
#define SCHED_TEST_H_

#define DLL_IMPORTS

#include "run_test.h"
#include "so_scheduler.h"
#include <stdio.h>

#define SO_TEST_FAIL	0
#define SO_TEST_SUCCESS	1
#define SO_MAX_UNITS	32

/* functions tested */
extern void test_sched_01(void);
extern void test_sched_02(void);
extern void test_sched_03(void);
extern void test_sched_04(void);
extern void test_sched_05(void);
extern void test_sched_06(void);
extern void test_sched_07(void);
extern void test_sched_08(void);
extern void test_sched_09(void);
extern void test_sched_10(void);
extern void test_sched_11(void);
extern void test_sched_12(void);
extern void test_sched_13(void);
extern void test_sched_14(void);
extern void test_sched_15(void);
extern void test_sched_16(void);
extern void test_sched_17(void);
extern void test_sched_18(void);
extern void test_sched_19(void);
extern void test_sched_20(void);
extern void test_sched_21(void);
extern void test_sched_22(void);

/* debugging macro */
#ifdef SO_VERBOSE_ERROR
#define so_error(msg, ...) fprintf(stderr, "ERR: " msg "\n", ##__VA_ARGS__)
#else
#define so_error(msg, ...)
#endif

/* shows the message and exits */
#define so_fail(msg) \
	do { \
		so_error(msg); \
		exit(-1); \
	} while (0)

/* returns unsigned random value between _min and _max - 1 */
#define get_rand(min, max) ((rand() % (max - min)) + min)

/* architecture dependent functions */
#ifdef __linux__

static inline tid_t get_tid(void)
{
	return pthread_self();
}

static inline int equal_tids(tid_t t1, tid_t t2)
{
	return pthread_equal(t1, t2);
}

/* useful defines */
static inline int this_tid(tid_t t)
{
	return pthread_equal(t, get_tid());
}

#elif defined _WIN32

#define inline __inline

static inline tid_t get_tid(void)
{
	return GetCurrentThreadId();
}

static inline int equal_tids(tid_t t1, tid_t t2)
{
	return t1 == t2;
}

/* useful defines */
static inline int this_tid(tid_t t)
{
	return equal_tids(t, get_tid());
}

static inline void sched_yield(void)
{
	Sleep(0);
}

#else
#error "Unknown platform"
#endif

#endif /* SCHED_TEST_H_ */

