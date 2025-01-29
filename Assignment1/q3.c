/* 
  Mohammad Omar Zahir - zahirm1
  Mahad Aziz - azizm17
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Initialize 5 threads for each philosopher
pthread_t th[5];
//Initialize a single mutex controlling all of the forks
pthread_mutex_t forks;
//Initialize a condition variable with 5 instances
pthread_cond_t cond[5];
//Initialize 4 possibilities for each of the 5 states for each philosopher
enum {THINKING, HUNGRY, EATING} state[5];

int r;

//Initialize a test function as done in the monitor solution to ensure that the philosophers to the left and right are not eating
void *test(int i) {
    if ((state[(i + 4) % 5] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % 5] != EATING)) {
        state[i] = EATING;
        pthread_cond_signal(&cond[i]);
    }
}

//Function for the philosophers putting down the fork
void *return_forks(int i) {
    state[i] = THINKING;
    //Check if the left and right philosopher can start eating
    test((i + 4) % 5);
    test((i + 1) % 5);
    printf("Philosopher %d is returning forks...\n", i);
}

//Function for the philosophers picking up the fork
void *pickup_forks(int phil) {
    //Locks the mutex forks all at once
    //While we could have implemented separate fork mutexes, having a single mutex will accomplish the same result with less overhead
    pthread_mutex_lock(&forks);
    //Change the state of the philosopher to hungry
    state[phil] = HUNGRY;
    //Check if philosopher can eat
    test(phil);
    //Continue checking if the waiting philosopher can begin eating
    while (state[phil] != EATING){
        pthread_cond_wait(&cond[phil], &forks);
    }
    printf("Philosopher %d is picking forks...\n", phil);
    printf("Philosopher %d is eating...\n", phil);
    srand(time(NULL));   // Initializes a seed for the random number
    r = (rand() % 3) + 1;      //Generates a number between 1 and 3
    sleep(r);
    //Unlocks the mutex after the fork has been picked up
    pthread_mutex_unlock(&forks);
}

//Function that creates the philosophers
void *philosopher(void* j) {
    int *phil = (int*) j;
    for(int i = 0; i < 5; i++){
        printf("Philosopher %d is thinking...\n", phil);
        srand(time(NULL));   // Initializes a seed for the random number
        r = (rand() % 3) + 1;      //Generates a number between 1 and 3  
        sleep(r);
        pickup_forks(phil);
        return_forks(phil);
    }
}

int main(void) {
    pthread_mutex_init(&forks, NULL);
    pthread_cond_init(&cond, NULL);
    for (int i = 0; i < 5; i++) {
        pthread_create(&th[i], NULL, &philosopher, i);
        state[i] = THINKING;
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(th[i], NULL);
    }
    pthread_mutex_destroy(&forks);
    pthread_cond_destroy(&cond);

    return 0;
}