#include <pthread.h>
#include <stdio.h>

#include <stdlib.h>

#define NUM_THREADS 10

int sum; // shared by threads
void *runner(void *param); // threads call this function

int main(int argc, char *argv[]) {
    pthread_t tid[NUM_THREADS]; // the thread identifier
    pthread_attr_t attr; // set of thread attributes

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_attr_init(&attr);
        // create the thread
        pthread_create(&tid[i], &attr, runner, argv[1]);
    }
    // se the default atts of the thread
    // pthread_attr_init(&attr);
    // create the thread
    // pthread_create(&tid, &attr, runner, argv[1]);
    // wait for the thread to exit
    for (int i = 0; i< NUM_THREADS; i++) {
        pthread_join(tid[i], NULL)
    }
    // pthread_join(tid, NULL);

    printf("sum = %d\n", sum);
}

// The thread will execute in this function
void *runner(void *param) {
    int i, upper = atoi(param);
    sum = 0;

    for (i = 1; i <= upper; i++) {
        sum += i;
    }

    pthread_exit(0);
}