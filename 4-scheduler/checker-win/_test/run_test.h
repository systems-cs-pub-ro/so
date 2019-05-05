/*
 * Testing framework headers
 *
 * 2012, Operating Systems
 */

#ifndef RUN_TEST_H_
#define RUN_TEST_H_

/* functions exported by the framework */
extern void init_test(void);
extern void cleanup_test(void);
extern void basic_test(int test_passed);

/* prototype executed for each test */
typedef void (run_test_f)(void);

struct run_test_t {
	run_test_f *function;	/* testing function */
};

/* tests array with each test */
extern struct run_test_t test_fun_array[];

#endif /* RUN_TEST_H_ */

