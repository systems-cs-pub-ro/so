#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int counter = 0;

void run() {
    srandom(time(NULL));

    // max 0.4 s
    usleep((random() % 400000));

    // let's check how many times this we called this function;
    counter++;
    printf("Got called %d times, cool!\n", counter);

    // max 0.4 s
    usleep((random() % 400000));
}

void reset() {
    // let's erase the history, shall we?
    counter = 0;
    printf("Reseted the counter!\n");
}
