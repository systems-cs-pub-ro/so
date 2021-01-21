#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N         10

static void *thread_code(void *arg)
{
    printf("Hello from the user thread code\n");
    return NULL;
}

int main()
{
    pthread_t threads[N];
    int i;

    for (i = 0 ; i < N ; i++)
        pthread_create(&threads[i], NULL, thread_code, (void *) i);

    for (i = 0 ; i < N ; i++)
        pthread_join(threads[i], NULL);

    return 0;
}

