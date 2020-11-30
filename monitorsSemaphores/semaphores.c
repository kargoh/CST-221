#include <unistd.h>    
#include <sys/types.h>  
#include <errno.h>      
#include <stdio.h>      
#include <stdlib.h>     
#include <pthread.h>    
#include <string.h>     
#include <semaphore.h>
// Monitors and Semaphores
// Developer: Isaiah DeBenedetto

// initialize thread routine
void threader ( void *ptr );

// initialize global variables
sem_t semaphore;
int counter; 

// main method to initialize the semaphore, create the new threads, join them, and then destroy and exit
int main()
{
    // initialize people in pool as an array
    int people[90];

    // initialize pools
    pthread_t pool1, pool2, pool3;

    // assign the people numbers to the array
    for(int i = 1; i <= 90; i++){
        people[i] = i;
    }  
    
    // initialize unique semaphore 
    sem_init(&semaphore, 0, 1);      

    // initialize the pools as threads
    pthread_create (&pool1, NULL, (void *) &threader, (void *) &people[0]);
    pthread_create (&pool2, NULL, (void *) &threader, (void *) &people[1]);
    pthread_create (&pool3, NULL, (void *) &threader, (void *) &people[2]);
                             
    // join pools
    pthread_join(pool1, NULL);
    pthread_join(pool2, NULL);
    pthread_join(pool3, NULL);

    // print people in pools
    printf("Pools are open.\n %d people are in the pool\n", counter);

    // destroy
    sem_destroy(&semaphore);
                  
    // exit
    exit(0);
}

void threader ( void *ptr )
{ 
    // initialize variables
    int x; 

    // assign variable to thread
    x = *((int *) ptr);

    // move through the semaphore
    printf("\n45 people going into the water %d\n", x);
    sem_wait(&semaphore);       

    // start critical region
    printf("People are entering pool %d...\n", x);
    // maximum pool capcity is 45 people
    for(int i = 0; i < 45; i++){
        printf("Person is entering pool %d\n", x);
        counter++;
        printf("Total People: %d\n", counter);
    }
    // end critical region 

    // move through semaphore
    sem_post(&semaphore);       
    
    // exit
    pthread_exit(0); 
}