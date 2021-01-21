/**
 * SO, 2019
 * Lab #8
 *
 * Task #3, lin
 *
 * Is malloc thread safe?
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <utils.h>
#include "critical.h"

#define NUM_THREADS 10
#define NUM_ROUNDS  10000
#define NUM_ROWS    100
#define NUM_COLUMNS 1000

#define PRINT_HDR_SZ    32
#define PRINT_BUF_SZ    256
#define MAIN_PRINT      (-1)

int global_storage;

static void increase_numbers(int *a, int *b, int*c) {
    (*a)++;
    (*b)++;
    (*c)++;
}

static void print_stats(int origin,
                        int *global_storage,
                        int *function_global_storage,
                        int *function_specific_storage)
{
    char hdr[PRINT_HDR_SZ] = { 0 };
    char buf[PRINT_BUF_SZ] = { 0 };

    /* TODO1: Enter critical section using acquire_lock */

    if (origin == MAIN_PRINT)
        sprintf(hdr, "Final report:\n");
    else
        sprintf(hdr, "Thread %d report:\n", origin);

    snprintf(buf, strlen(hdr) + 1, "%s\n", hdr);

    sprintf(buf + strlen(buf),
            "sum_of(global_storage)=%d\n", *global_storage);
    sprintf(buf + strlen(buf),
            "sum_of(function_global_storage)=%d\n", *function_global_storage);
    sprintf(buf + strlen(buf),
            "sum_of(function_specific_storage)=%d\n", *function_specific_storage);

    printf("%s", buf);

    /* TODO1: Leave critical section using release_lock */
}

void *thread_function(void *args)
{
    static int function_global_storage;
    int function_specific_storage = 0;
    size_t i, j;
    char **a;

    for (int i = 0; i < NUM_ROUNDS; i++) {

        /* TODO1: Enter critical section using acquire_lock */

        increase_numbers(&global_storage,
                         &function_global_storage,
                         &function_specific_storage);

        /* TODO1: Leave critical section using release_lock */
    }

    print_stats((long)args,
                &global_storage,
                &function_global_storage,
                &function_specific_storage);

    a = malloc(1000 * sizeof(char *));
    for (i = 0; i < NUM_ROWS; i++) {
        a[i] = malloc(NUM_COLUMNS);

        for (j = 0; j < NUM_COLUMNS; j++)
            a[i][j] = rand() % (NUM_COLUMNS * NUM_ROWS);
    }

    return a;
}

void free_mat(void **mat)
{
    /* Free the memory allocated by each thread */

    size_t i;
    char **a = (char **) mat;

    for (i = 0; i < NUM_ROUNDS/100; i++)
        free(a[i]);
    free(a);
}

int main(void)
{
    static int function_global_storage;
    int function_specific_storage = 0;

    pthread_t th[NUM_THREADS];
    int rc;
    size_t i;
    void *mat[NUM_THREADS];

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&th[i], NULL, thread_function, (void *) i);
        DIE(rc != 0, "pthread_create");
    }

    for (i = 0; i < NUM_ROUNDS; i++) {

        /* TODO1: Enter critical section using acquire_lock */

        increase_numbers(&global_storage,
                         &function_global_storage,
                         &function_specific_storage);

        /* TODO1: Leave critical section using release_lock */
    }

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(th[i], &mat[i]);
        DIE(rc != 0, "pthread_join");
    }

    /* Freeing */
    for (i = 0; i < NUM_THREADS; i++)
        free_mat(mat[i]);

    /* Extract final report */
    print_stats(MAIN_PRINT,
                &global_storage,
                &function_global_storage,
                &function_specific_storage);

    return 0;
}
