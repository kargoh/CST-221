#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
// Producer and Consumer
// Developer: Isaiah DeBenedetto

// initialize variables
int MAX = 100;
int WAKEUP = SIGUSR1;
int SLEEP = SIGUSR2;

// initialize pid1 (parent or child process)
pid_t pid1;

// set signal
sigset_t sigSet;

// ciruclar buffer method
struct CIRCULAR_BUFFER
{
    int count;          // Count of items in buffer
    int lower;          // The lower slot of the  buffer
    int upper;          // Upper slot of the buffer
    int buffer[100];
};
struct CIRCULAR_BUFFER *buffer = NULL;

// sleep until signaled to wake up
void sleepAndWait()
{
    int nSig;
    printf("Sleep state\n");
    sigwait(&sigSet, &nSig);
    printf("Awake state\n");
}

// wake up other by killing pid1 and WAKEUP
void wakeUpOther()
{
    kill(pid1, WAKEUP);
}wwwwwwww
wwwwwwww
// get value from bufferwwwwwwww
int getValue()wwwwwwww
{wwwwwwww
    int item = buffer->buffer[buffer->lower];wwwwwwww
wwwwwwww
    buffer->count = buffer->count - 1;

    // check if buffer should go to 0, if not then increase buffer by 1
    if(buffer->lower == 99){
        buffer->lower = 0;
    } else {
        buffer->lower = buffer->lower + 1;
    }
	
    // return value
    return item;
}

// put value back in buffer
void putValue(struct CIRCULAR_BUFFER* buffer, int value)
{
    // add int into upper slot
    buffer->buffer[buffer->upper] = value;

    // increase buffer count by 1
    buffer->count = buffer->count + 1;

    // check if buffer should go to 0, if not then increase buffer by 1
    if(buffer->upper == 99){
        buffer->upper = 0;
    } else {
        buffer->upper = buffer->upper + 1;
    }
}

// **consumer process**
void consumer()
{
    // set sigSet to look for WAKEUP
    sigemptyset(&sigSet);
    sigaddset(&sigSet, WAKEUP);
    sigprocmask(SIG_BLOCK, &sigSet, NULL);

    // start consumer process
    printf("Consumer process starting...\n");
    
    // consumer while loop logic
    while(true){
        if(buffer->count <= 0) sleepAndWait();
        int item = getValue();

        // consume item
        printf("Item consumed: %d\n", item);
        sleep(2);
        if(buffer->count < 100) wakeUpOther();
    }
    
    _exit(1);
}

// **producer process**
void producer()
{
    // set sigSet to look for WAKEUP
    sigemptyset(&sigSet);
    sigaddset(&sigSet, WAKEUP);
    sigprocmask(SIG_BLOCK, &sigSet, NULL);

    // value to add to buffer
    int value = 0;

    // start producer process
    printf("Producer process starting..\n");
    
    // producer while loop logic (found in page 129 in textbook)
    while(true){
        if(buffer->count == MAX - 1) sleepAndWait();

        printf("Producer adding value: %d to buffer\n", value);
        putValue(buffer, value);

        sleep(1);

        // produce item
        printf("Buffer count %d\n", buffer->count);

        value = value + 1;
        if(buffer->count > 0) wakeUpOther();
    }
    

    // exit
    _exit(1);
}

// **forking process and main argument**
int main(int argc, char* argv[])
{
    pid_t  pid;

    // shared memory for the buffer
    buffer = (struct CIRCULAR_BUFFER*)mmap(0,sizeof(buffer), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    buffer->count = 0;
    buffer->lower = 0;
    buffer->upper = 0;

    // initialize fork
    pid = fork();
    // if fork returns -1
    if (pid == -1)
    {
        // error out
        printf("Fork not allowed, error %d\n", errno);
        exit(EXIT_FAILURE);
    }
    
    // if fork returns 0 int then the child process is ran
    // if fork returns any positive int then the parent process is ran
    if (pid == 0)
    {
        //Producer process being ran as a child process
        pid1 = getppid();
        producer();
    }
    else
    {
        //Consumer process being ran as a parent process
        pid1 = pid;
        consumer();
    }

    return 0;
}