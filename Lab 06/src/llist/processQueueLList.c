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
	int processIndex = 0;
	while (processIndex < processTableSize)
		if (processTable[processIndex++].burstTime != 0)
			return true;
	return false;
}

/***
 * Adds any processes that arrive at the current time to ready queue
 */
void addArrivingProcessesToReadyQueue(int time)
{
	// TODO: Done
	PROCESS* process = &processTable[processTableSize];
	if (process->entryTime == time)
		addProcessToReadyQueue(&processTable[processTableSize++]);
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
	// Empty Ready Queue or a Null Process
	if (NULL == readyQueueHead || NULL == process)
		return;
	// Process is Head Node
	if (readyQueueHead == process)
		readyQueueHead = readyQueueHead->next;
	// Process is Not Tail Node
	if (NULL != process->next)
		process->next->previous = process->previous;
	// Process is Not Head Node
	if (NULL != process->previous)
		process->previous->next = process->next;
	free(process);
}

/***
 * Fetches the first process from the ready queue
 */
PROCESS* fetchFirstProcessFromReadyQueue()
{
	// TODO: Done
	return readyQueueHead;
}

/***
 * Finds the shortest job in the ready queue
 */
PROCESS* findShortestProcessInReadyQueue()
{
	// TODO: Done
	PROCESS* processIter = readyQueueHead;
	PROCESS* shortestProcess = readyQueueHead;
	while (NULL != processIter)
	{
		if (processIter->burstTime == 0)
		{
			PROCESS* processIterNext = processIter->next;
			removeProcessFromReadyQueue(processIter);
			processIter = processIterNext;
		}
		else if (shortestProcess->burstTime < processIter->burstTime)
		{
			shortestProcess = processIter;
			processIter = processIter->next;
		}
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

	if (NULL != readyQueueHead)
		return;

	for (int processIndex = 0; processIndex < processTableSize; ++processIndex)
		free(&processTable[processIndex]);
	free(processTable);
}





