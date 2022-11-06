#
#  4 - Thread Scheduler (Linux)
#
#  2017, Operating Systems
#

# Archive:

> Makefile.checker
> README
> run_all.sh
> _test/
	> run_test,c
	> run_test.h
	> scheduler_test.h
	> so_scheduler.h
	> test_exec.c
	> test_io.c
	> test_sched.c

# Build:

The local directory must contain the thread scheduler library
(libscheduler.so). Use the Makefile.checker to properly build the run_test
executable:

	make -f Makefile.checker

This command will also run all tests and print out the results.

# Run:

In order to run the test suite you can either use the run_all.sh script or
the run_test executable. The loader must be able to locate the library
(libscheduler.so) (use LD_LIBRARY_PATH).

The run_all.sh script runs all tests and computes assignment grade (95 points
maximum):

	LD_LIBRARY_PATH=. ./run_all.sh

In order to run a specific test, pass the test number (1 .. 20) to the run_test
executable:

	LD_LIBRARY_PATH=. ./_test/run_test 1

# Debug:

For debugging, define the SO_VERBOSE_ERROR symbol.

