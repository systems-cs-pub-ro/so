/**
 * SO, 2010 - Laborator #13
 * Threads counter - analyze what happends if we use fork() into the thread code.
 * The usage of this program is:
 * ./counter nthreads should_fork, with default values 100 and 1
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

#define CHECK(x) \
    do { \
        if (!(x)) { \
            perror(#x); \
            exit(1); \
        } \
    } while(0)

static sem_t sem;
static char buffer[128];
static int should_fork = 1;

static void *thread_code(void *args)
{
    pid_t pid;
    int status;

    if (!args && should_fork)
    {
        switch((pid = fork()))
        {
            case -1:
                exit(-1);
            case 0:
                system("./counter 10 0");
                sleep(5);
                exit(-1);
            default:
                waitpid(pid, &status, 0);
        }
    }

    CHECK(sem_wait(&sem) == 0);
    return NULL;
}

static int get_threads_number(char *cmd)
{
    int pipes[2];
    pid_t pid;
    int status;

    CHECK(pipe(pipes) == 0);

    switch((pid = fork()))
    {
        case -1:
            exit(-1);
        case 0:
        {
            // Proces copil
            
            // Pun comanda sa scrie in pipe-ul meu
            close(pipes[0]);
            dup2(pipes[1], STDOUT_FILENO);

            const char *argv[] = {"/bin/bash", "-c", cmd, NULL};
            execv("/bin/bash", (char *const *)argv);
            exit(-1);
        }
        default:
        {
            // Proces parinte

            // Astept terminarea procesului copil
            close(pipes[1]);
            waitpid(pid, &status, 0);

            // Citesc datele de pe pipe
            memset(buffer, 0, 128);
            CHECK(read(pipes[0], buffer, 128) > 0);
            close(pipes[0]);
            break;
        }
    }

    // Scadem 3 pentru ca in rezultat apar: . .. si total x
    return atoi(buffer) - 3;
}

int main(int argc, char **argv)
{
    int i, N = 100, nthreads;
    pthread_t *threads;

    if (argc > 1)
        N = atoi(argv[1]);

    if (argc > 2)
        should_fork = atoi(argv[2]);

    CHECK(sem_init(&sem, 0, 0) == 0);
    threads = malloc(N * sizeof(pthread_t));
    if (!threads)
        return -1;

    for (i = 0 ; i < N ; i++)
        CHECK(pthread_create(&threads[i], NULL, thread_code, (void *) i) == 0);

    snprintf(buffer, 128, "ls -la /proc/%d/task | wc -l", (int) getpid());

    nthreads = get_threads_number(buffer);
    printf("Numarul de task-uri este %d\n", nthreads);

    for (i = 0 ; i < N ; i++)
        CHECK(sem_post(&sem) == 0);

    sleep(3);

    for (i = 0 ; i < N ; i++)
        CHECK(pthread_join(threads[i], NULL) == 0);

    free(threads);
    CHECK(sem_destroy(&sem) == 0);

    return 0;
}

