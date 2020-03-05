#include "processQueue.h"

PROCESS *processTable;
int processTableSize = 0;
int processTableCapacity;

// TODO: use a linked list for the ready queue

PROCESS **readyQueue;
int readyQueueSize = 0;
int readyQueueCapacity;

/***
 * constructor of the process table
 */
void createProcessTable(int capacity)
{
    processTable = (PROCESS *) malloc(capacity * sizeof(PROCESS));
    processTableCapacity = capacity;
}

/***
 * constructor of the ready queue
 */
void createReadyQueue(int capacity)
{
    readyQueue = (PROCESS **) malloc(capacity * sizeof(PROCESS *));
    readyQueueCapacity = capacity;
}

/***
 * adds a process and expands the table if necessary
 */
void addProcessToTable(PROCESS process)
{
    if (processTableSize >= processTableCapacity) //if array too small
    {
        processTableCapacity *= 2; //double capacity
        processTable = (PROCESS *) realloc(processTable, processTableCapacity * sizeof(PROCESS));
    }

// TODO: complete
}

/***
 * display the processes table
 */
void displayProcessTable()
{
    printf("PROCESSES:\n\nName    \tEntry\tBurst\n");
    for (int i = 0; i < processTableSize; i++)
    {
        printf("%-8s\t%3d   \t%3d   \n", processTable[i].name, processTable[i].entryTime, processTable[i].burstTime);
    }
    printf("\n");
}

/***
 * determines if any processes in the process table still need to execute
 */
//will determine if any processes in the process queue have more to execute
bool processesLeftToExecute()
{
// TODO: implement

    return false; //return 0 if all processes are complete
}

/***
 * adds any processes that arrive at the current time to ready queue
 */
void addArrivingProcessesToReadyQueue(int time)
{
// TODO: implement
}

/***
 * adds a process to the ready queue and expands it if necessary
 */
void addProcessToReadyQueue(PROCESS *pointer)
{
    if (readyQueueSize >= readyQueueCapacity) //if array too small
    {
        readyQueueCapacity *= 2; //double capacity
        readyQueue = (PROCESS **) realloc(readyQueue, readyQueueCapacity * sizeof(PROCESS *));
    }

// TODO: complete
}

/***
 * removes a process from the ready queue and fills the "hole"
 */
void removeProcessFromReadyQueue(PROCESS *p)
{
// TODO: implement

    readyQueueSize--;
}

/***
 * fetches the first process from the ready queue
 */
PROCESS *fetchFirstProcessFromReadyQueue()
{
// TODO: implement

    return NULL;
}

/***
 * finds the shortest job in the ready queue
 */
PROCESS *findShortestProcessInReadyQueue()
{
// TODO: implement

    return NULL;
}

/***
 * displays the contents of the ready queue
 */
void displayQueue()
{
    int i;
    printf("QUEUE: ");

    if (readyQueueSize <= 0)
        printf("<empty>");
    else
    {
        for (i = 0; i < readyQueueSize; i++)
        {
            printf("%s(%d) ", readyQueue[i]->name, readyQueue[i]->burstTime);
        }
    }
}

/***
 * calculates and prints the average wait time using information in the process table
 */
void printAverageWaitTime()
{
    int i = 0;
    double sum = 0;
    for (i = 0; i < processTableSize; i++)
    {
        sum = sum + processTable[i].waitTime;
        printf("Process %s Wait Time: %.2lf\n", processTable[i].name, (double) processTable[i].waitTime);
    }
    printf("Average Wait Time: %.2lf\n", (sum / (double) processTableSize));
}

/***
 * clean up the process table
 */
void cleanUp()
{
// TODO: implement
}





