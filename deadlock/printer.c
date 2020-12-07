#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>
// Monitors and Semaphores
// Developer: Isaiah DeBenedetto

// initiate global variables
sem_t* semaphore;
pid_t otherPid;
sigset_t sigSet;

void signalHandler(int signalNumber)
{
    printf("Print process continues.\n");
}

// method to process printing
void printProcess() 
{

    // initiate signal
    signal(SIGUSR1, signalHandler);

    // begin child process (hung process waiting for semaphore)
    int value;
    sem_getvalue(semaphore, &value);
    printf("Process: Semaphore count is %d.\n", value);
    printf("Process: Getting semaphore.\n");

    // make semaphore wait(lock) so the print process can run
    sem_wait(semaphore);
    sem_getvalue(semaphore, &value);
    printf("Process: Semaphore count is %d.\n", value);

    // Critical Region begins
    printf("Process: Print process starts.\n");
    int val = 0;
    for(;;) 
    {
        printf("Printing.\n");
        sleep(1);
        val = val + 1;
        if(val == 60)
        {
            printf("Completed Printing.\n");
            break;
        }
    }
    // Critical Region ends
    sem_post(semaphore);

    // exit
    printf("Process: Print process ends.");
    _exit(0);
}

// checkQueue method is to check if the semaphore is waiting(locked)
// using the 10 second timer, if the print process isn't completed in the alotted timeframe then the process is killed
void* checkQueue(void *a) 
{
    // initiate variables
    int* status = a;

    printf("Checking for hung print process....\n");
    sleep(10); // wait 10 seconds
    if(sem_trywait(semaphore) != 0) 
    {
        
        printf("System Call: Killing Process");
        *status = 1;
        
    } 
    else 
    {
        printf("Print Process is running fine ....\n");
        *status = 0;
    }
    return NULL;
}

// printParent method is to 
void printParent() 
{

    sleep(2);
    if(getpgid(otherPid) >= 0) 
    {
        printf("Print Parent: Started while print process is still running!\n");
    }
    int value;
    sem_getvalue(semaphore, &value);
    printf("Print Parent: Semaphore count of %d.\n", value);

    // check to see if the semaphore is locked
    if(sem_trywait(semaphore) != 0)
    {
        // initiate variables
        pthread_t tid1;
        int status = 0;

        // timer thread to check if process is hung
        printf("Print Parent: print process is either hung or taking too long\n");
        if(pthread_create(&tid1, NULL, checkQueue, &status)) 
        {
            printf("ERROR while creating timer thread.\n");
            _exit(1);
        }
        if(pthread_join(tid1, NULL)) 
        {
            printf("\n ERROR while joining timer thread.\n");
            exit(1);
        }

        // if semapgore is locked kill the print process
        if(status == 1) 
        {
            printf("Killing process with ID of %d\n", otherPid);
            kill(otherPid, SIGTERM);

            printf("Process killed successfully\n");
            sleep(5);
            kill(otherPid, SIGUSR1);
            sleep(1);

            sem_getvalue(semaphore, &value);
            printf("Print Parent: Semaphore count of %d.\n", value);
            if (sem_trywait(semaphore) != 0) 
            {
                if(value == 0) 
                {
                    sem_post(semaphore);
                }
                printf("Semaphore has been retrieved.\n");
                sem_getvalue(semaphore, &value);
                printf("Print Parent: Semaphore count of %d. \n", value);
            }
            else 
            {
                printf("Semaphore has been retrieved.\n");
            }
        }
    }

    printf("Exiting Print Parent.\n");
    _exit(0);
  
}


// main method
int main(int argc, char* argv[]) 
{

    pid_t pid;
    // initiate global semaphore
    semaphore = (sem_t*)mmap(0,sizeof(sem_t), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    if(sem_init(semaphore, 1, 1) != 0) 
    {
        printf("Failed to create semaphore");
        exit(EXIT_FAILURE);
    }

    // set setvbuf
    setvbuf(stdout, NULL, _IOLBF, 0);

    // set fork
    pid = fork();
    if(pid == -1) 
    {
        //Error: if fork() retrns -1 
        fprintf(stderr, "Cant fork, error %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // if fork() returns 0 
    if(pid == 0) 
    {
        //Run child process logic
        printf("Printer process started | pid: %d\n", getpid());
        otherPid = getppid();
        printProcess();
    } else 
    {
        //Run Parent process logic
        printf("Printer manager process started | pid: %d\n", getpid());
        otherPid = pid;
        printParent();
    }

    //Cleanup
    sem_destroy(semaphore);

    //Return OK
    return 0;
    
}