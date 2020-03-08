/**
* Name: Ferid Ruano
* Lab/task: Lab 06
* Date: 03/5/2020
**/

#include "processQueue.h"

// The process table to hold all processes
PROCESS* processTable;
int processTableSize = 0;
int processTableCapacity;

// Doubly-linked list to hold the ready queue
PROCESS* readyQueueHead = NULL;
PROCESS* readyQueueTail = NULL;

/***
 * Constructor of the process table
 */
void createProcessTable(int capacity)
{
	processTable = (PROCESS*)malloc(capacity * sizeof(PROCESS));
	processTableCapacity = capacity;
}

/***
 * Constructor of the ready queue
 */
void createReadyQueue(int capacity)
{
	readyQueueHead = NULL;
	readyQueueTail = NULL;
}

/***
 * Adds a process and expands the table if necessary
 */
void addProcessToTable(PROCESS process)
{
	// Resize Process Table When Full
	if (processTableSize >= processTableCapacity)
	{
		processTableCapacity *= 2; // Double Process Table Capacity
		processTable = (PROCESS*)realloc(processTable, processTableCapacity * sizeof(PROCESS));
	}
	// TODO: Done
	processTable[processTableSize++] = process;
}

/***
 * Display the processes table
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
 * Determines if any processes in the process table still need to execute
 */
bool processesLeftToExecute()
{
	// TODO: Done
	int processTableIndex = 0;
	while (processTableIndex < processTableSize)
		if (processTable[processTableIndex++].burstTime != 0)
			return true;
	return false;
}

/***
 * Adds any processes that arrive at the current time to ready queue
 */
void addArrivingProcessesToReadyQueue(int time)
{
	// TODO: Done
	for (int processIterIndex = 0; processIterIndex < processTableSize; ++processIterIndex)
		if (processTable[processIterIndex].entryTime == time)
			addProcessToReadyQueue(&processTable[processIterIndex]);
}

/***
 * Adds a process to the ready queue and expands it if necessary
 */
void addProcessToReadyQueue(PROCESS* process)
{
	// TODO: Done
	if (NULL == readyQueueHead)
	{
		readyQueueHead = process;
		readyQueueTail = process;
	}
	else
	{
		readyQueueTail->next = process;
		process->previous = readyQueueTail;
		readyQueueTail = process;
	}
}

/***
 * Removes a process from the ready queue and fills the "hole"
 */
void removeProcessFromReadyQueue(PROCESS* process)
{
// TODO: Done
	// Empty Ready Queue, Null Process, or Check Tail for Initialization Error
	if (NULL == readyQueueHead || NULL == readyQueueTail || NULL == process)
		return;
	// Process is Head Node
	if (readyQueueHead == process)
		readyQueueHead = process->next;
	// Process is Tail Node
	if (readyQueueTail == process)
		readyQueueTail = process->previous;
	// Process is Not Tail Node
	if (NULL != process->next)
		process->next->previous = process->previous;
	// Process is Not Head Node
	if (NULL != process->previous)
		process->previous->next = process->next;
}

/***
 * Fetches the first process from the ready queue
 */
PROCESS* fetchFirstProcessFromReadyQueue()
{
	// TODO: Done
	PROCESS* processIter = readyQueueHead;

	// Remove Terminated Processes and Return First Non-Terminated Process
	while (NULL != processIter)
	{
		if (processIter->burstTime != 0)
			break;

		PROCESS* processIterNext = processIter->next;
		removeProcessFromReadyQueue(processIter);
		processIter = processIterNext;
	}
	return processIter;
}

/***
 * Finds the shortest job in the ready queue
 */
PROCESS* findShortestProcessInReadyQueue()
{
	// TODO: Done
	// Traverse the Linked-List Twice (Not as efficient, but gets the job done)
	PROCESS* processIter = readyQueueHead;

	// Remove Terminated Processes
	while (NULL != processIter)
	{
		if (processIter->burstTime == 0)
			removeProcessFromReadyQueue(processIter);
		processIter = processIter->next;
	}

	// Find Shortest Process
	processIter = readyQueueHead;
	PROCESS* shortestProcess = readyQueueHead;
	while (NULL != processIter)
	{
		if (shortestProcess->burstTime > processIter->burstTime)
			shortestProcess = processIter;
		processIter = processIter->next;
	}
	return shortestProcess;
}

/***
 * Displays the contents of the ready queue
 */
void displayQueue()
{
	printf("QUEUE: ");

	if (readyQueueHead == NULL)
		printf("<empty>");
	else
	{
		for (PROCESS* curr = readyQueueHead; curr != NULL; curr = curr->next)
		{
			printf("%s(%d) ", curr->name, curr->burstTime);
		}
	}
}

/***
 * Calculates and prints the average wait time using information in the process table
 */
void printAverageWaitTime()
{
	int i = 0;
	double sum = 0;
	for (i = 0; i < processTableSize; i++)
	{
		sum = sum + processTable[i].waitTime;
		printf("Process %s Wait Time: %.2lf\n", processTable[i].name, (double)processTable[i].waitTime);
	}
	printf("Average Wait Time: %.2lf\n", (sum / (double)processTableSize));
}

/***
 * Clean up the process table
 */
void cleanUp()
{
	// TODO: Done
	// Empty Ready Queue
	if (NULL != readyQueueHead)
		return;
	// Free Process Table's Array of PROCESS elements
	for (int processIndex = 0; processIndex < processTableSize; ++processIndex)
		free(&processTable[processIndex]);
	free(processTable);
}





