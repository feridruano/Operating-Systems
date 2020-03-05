#include "processQueue.h"

// the process table to hold all processes
PROCESS *processTable;
int processTableSize = 0;
int processTableCapacity;

// doubly-linked list to hold the ready queue
PROCESS *readyQueueHead = NULL;
PROCESS *readyQueueTail = NULL;

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
    readyQueueHead = NULL;
    readyQueueTail = NULL;
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
void addProcessToReadyQueue(PROCESS *process)
{
// TODO: implement
}

/***
 * removes a process from the ready queue and fills the "hole"
 */
void removeProcessFromReadyQueue(PROCESS *process)
{
// TODO: implement
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
    printf("QUEUE: ");

    if (readyQueueHead == NULL)
        printf("<empty>");
    else
    {
        for (PROCESS *curr = readyQueueHead; curr != NULL; curr = curr->next)
        {
            printf("%s(%d) ", curr->name, curr->burstTime);
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





