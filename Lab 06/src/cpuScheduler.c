/**
* Name: Ferid Ruano
* Lab/task: Lab 06
* Date: 03/5/2020
**/

#include "cpuScheduler.h"

static int quantum;

int main(int argc, char** argv)
{

#ifdef _DEBUG
	if (argc > 1)
	{
		freopen(argv[1], "r", stdin);
		printf("Reading input from file %ss.\n\n", argv[1]);
	}
	else
		printf("Reading input from stdin.\n\n");
#endif

	int time = 0; // Simulated Time

	ALGORITHM_PARAMS parameters = {
		.cpu = NULL, .algorithm = "",
		.step = NULL, .quantum = 0,
		.time = 0
	}; // Simulation Parameters

	// Read the Algorithm Type and Time Quantum if Necessary
	scanf("%s", parameters.algorithm);

	// Check which Algorithm was Passed, Set Values Accordingly
	if (strcmp(parameters.algorithm, "RR") == 0)
	{
		scanf("%d", &parameters.quantum);
		quantum = parameters.quantum;
		parameters.step = &rrStep;
	}
	else if (strcmp(parameters.algorithm, "FCFS") == 0)
		parameters.step = &fcfsStep;
	else if (strcmp(parameters.algorithm, "SJF") == 0)
		parameters.step = &sjfStep;
	else if (strcmp(parameters.algorithm, "SRTF") == 0)
		parameters.step = &srtfStep;
	else
	{
		printf("The job type input is not a valid input!");
		exit(EXIT_FAILURE);
	}

	scanf("\n"); // Skip Over the End of Line Marker

	printf("\nALGORITHM: %s", parameters.algorithm);
	if (strcmp(parameters.algorithm, "RR") == 0)
		printf("%3d", parameters.quantum);
	printf("\n\n");

	createProcessTable(INITIAL_CAPACITY); //create process table
	createReadyQueue(INITIAL_CAPACITY); //create ready queue

	readProcessTable(); //populate global process table
	displayProcessTable();

	printf("SIMULATION:\n\n");

	while (processesLeftToExecute())
	{
		addArrivingProcessesToReadyQueue(time);

		parameters.time = time;

		doStep(parameters.step, &parameters);

		displayTimeTick(time, parameters.cpu);

		if (parameters.cpu != NULL)
			parameters.cpu->burstTime--;

		time++;
	}

	printAverageWaitTime();

	cleanUp();

	return EXIT_SUCCESS;
}

/***
 * step executor
 */
void doStep(void (* func)(void*), void* param)
{
	func(param);
}

/***
 * function implementing a step of FCFS
 */
void fcfsStep(void* param)
{
	ALGORITHM_PARAMS* p = (ALGORITHM_PARAMS*)param;

	//if the cpu has nothing currently executing
	if (p->cpu == NULL || p->cpu->burstTime == 0)
	{
		p->cpu = fetchFirstProcessFromReadyQueue(); //start executing the first process in the ready queue
		if (p->cpu != NULL)
			p->cpu->waitTime = p->time - p->cpu->entryTime; // update the wait time
	}
}

/***
 * function implementing a step of SJF
 */
void sjfStep(void* param)
{
	// TODO: Done
	ALGORITHM_PARAMS* p = (ALGORITHM_PARAMS*)param;

	// Non-Preemptive - Short Job First Until Terminated
	// Two Processes with Equal Entry Time, then Shortest Burst Time is First to Execute

	// CPU Not Currently Executing a Process
	if (NULL == p->cpu || 0 == p->cpu->burstTime)
	{
		// Start Executing Shortest
		p->cpu = findShortestProcessInReadyQueue();
		removeProcessFromReadyQueue(p->cpu);
		// Non-terminated Process, then Update WaitTime
		if (NULL != p->cpu)
			p->cpu->waitTime = p->time - p->cpu->entryTime;
	}
}

/***
 * function implementing a step of SRTF
 */
void srtfStep(void* param)
{
	// TODO: Done
	ALGORITHM_PARAMS* p = (ALGORITHM_PARAMS*)param;

	// Preemptive - Short Job First
	// Shortest Job First Will Always Be "Currently" Executing

	// CPU Not Currently Executing a Process
	if (NULL == p->cpu || 0 == p->cpu->burstTime)
	{
		p->cpu = findShortestProcessInReadyQueue();
		removeProcessFromReadyQueue(p->cpu);

		// Non-terminated Process, then Update WaitTime
		if (NULL != p->cpu)
			p->cpu->waitTime = p->time - p->cpu->offTime;
	}

	// Find Shortest Remaining Time Process
	PROCESS* shortestProcess = findShortestProcessInReadyQueue();
	if (NULL != shortestProcess && p->cpu->burstTime > shortestProcess->burstTime)
	{
		p->cpu->offTime = p->time; // Record Process OffTime
		removeProcessFromReadyQueue(shortestProcess);
		addProcessToReadyQueue(p->cpu);
		p->cpu = shortestProcess;
	}
}

/***
 * function implementing a step of RR
 */
void rrStep(void* param)
{
	// TODO: Done
	ALGORITHM_PARAMS* p = (ALGORITHM_PARAMS*)param;
	if (NULL == p->cpu || 0 == p->cpu->burstTime || 0 == p->quantum)
	{
		// Add Non-Terminated Processes with Quantum of Zero to Ready Queue
		if (NULL != p->cpu && 0 != p->cpu->burstTime)
		{
			p->cpu->offTime = p->time; // Record OffTime
			addProcessToReadyQueue(p->cpu);
		}

		// CPU Not Currently Executing a Process OR Process Terminated
		p->cpu = fetchFirstProcessFromReadyQueue();
		// Non-terminated Process, then Update WaitTime
		if (NULL != p->cpu)
			p->cpu->waitTime += p->time - p->cpu->offTime;
		// Reset Quantum Time
		p->quantum = quantum;
	}
	p->quantum--;
}

/***
 * Fills the Process Table with Processes from Input
 */
int readProcessTable()
{
	PROCESS tempProcess = {
		.name = "",
		.entryTime = 0,
		.burstTime = 0,
		.offTime = 0,
		.waitTime = 0,
		.previous = NULL,
		.next = NULL
	};

	char* line = NULL;
	char* currPos;
	size_t len = 0;

	int counter = 0;
	int offset = 0;

	while (getline(&line, &len, stdin) != -1)
	{
		currPos = line;
		sscanf(currPos, "%s%n", tempProcess.name, &offset);
		currPos += offset;
		sscanf(currPos, "%d%n", &tempProcess.entryTime, &offset);
		tempProcess.offTime = tempProcess.entryTime; // simplifies computation of the wait time
		currPos += offset;
		sscanf(currPos, "%d", &tempProcess.burstTime);

		tempProcess.previous = NULL;
		tempProcess.next = NULL;

		addProcessToTable(tempProcess);

		counter++;
	}

	free(line);

	return counter;
}

void displayTimeTick(int time, PROCESS* cpu)
{
	printf("T%d:\nCPU: ", time);
	if (cpu == NULL)
		printf("<idle>\n");
	else
		printf("%s(%d)\n", cpu->name, cpu->burstTime);

	displayQueue();
	printf("\n\n");
}






