/**
 * SO, 2019
 * Lab #8
 *
 * Task #3, lin
 *
 * Thread safety
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <utils.h>

/* *******************************
 * API to mark a critical region
 * */
#ifdef USE_SPINLOCK
static bool lock;
#else
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

void acquire_lock(void)
{
#ifdef USE_SPINLOCK
    /* Busy waiting */
    while(1) {

        /* TODO2: Implement spinlock lock method
         *
         * HINT: Use atomic intrutions. ex: __atomic_test_and_set.
         *
         * HINT 2: Use the most restrictive memory order for the
         * second parameter of __atomic_test_and_set. See the
         * https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
         * link and search for "__ATOMIC_SEQ_CST".*/




        /*
         * Short explanation: High-level language code is translated into
         * low-level code. The low level code is not guaranteed to follow
         * the same code flow as the high-level code. Some instructions may be
         * skipped or reordered for the sake of optimization!
         * For the computer, as long as the final output
         * is not affected, all's good. Memory order values such as "__ATOMIC_*"
         * ensure that the low-level code associated with calls such as
         * __atomic_test_and_set does not get reordered by limiting the amount
         * of optimizations done to the program.
         *
         * If you're interested in knowing more, see this link:
         * https://www.internalpointers.com/post/understanding-memory-ordering.
         */
    }
#else
    pthread_mutex_lock(&lock);
#endif
}

void release_lock(void)
{
#ifdef USE_SPINLOCK


#else
    pthread_mutex_unlock(&lock);
#endif
}
/*******************************
 * end of API */

