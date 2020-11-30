#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
// Monitors and Semaphores
// Developer: Isaiah DeBenedetto

// initialize variables
#define totalPeople 1000000
int peopleCounted = 0;
pthread_mutex_t mutex;

// create mutex to count people in line
void *countPeople(void *a)
{
    pthread_mutex_init(&mutex, 0);
    int x, tmp;
    for (x = 0; x < totalPeople; ++x)
    {
        // critical region
        pthread_mutex_lock(&mutex);

        tmp = peopleCounted;
        tmp = tmp + 1;
        peopleCounted = tmp;

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// method where all threasds are created, counted, and calculated
int main() 
{
    // create unique threads
    pthread_t thread1, thread2;
    
    // check for errors in creation
    if (pthread_create(&thread1, NULL, countPeople, NULL))
    {
        printf("\n ERROR creating thread 1");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, countPeople, NULL))
    {
        printf("\n ERROR creating thread 2");
        exit(1);
    }

    // count threads
    if (pthread_join(thread1, NULL))
    {
        printf("\n ERROR joining thread 1");
        exit(1);
    }

    if (pthread_join(thread2, NULL))
    {
        printf("\n ERROR joining thread 2");
        exit(1);
    }

    // if total calculated people equals the goal then it is right
    if (peopleCounted < (2 * totalPeople))
    {
        printf("\n The amount of people in line is wrong! Amount of People: %d\n", peopleCounted);
    }
    else 
    {
        printf("\n The amount of people in line is wrong! Amount of People: %d\n", peopleCounted);
    }

    pthread_mutex_destroy(&mutex);
    exit(1);
}