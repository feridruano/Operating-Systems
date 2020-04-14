/**
* Name: Ferid Ruano
* Lab/task: Lab 09
* Date: 04/13/2020
**/

#include "../inc/inverted.h"

// "hidden" symbols (i.e., not included in the .h file)

int tableSize;
int frameSize;

int lookUp(PROC*, int, int);
int findOldest(PROC* table);

/*
 * initializes the table for the given sizes of the memory and frame
 * all entries should be zeroed (e.g., calloc())
 */
void initInverted(PROC** table, int msize, int fsize)
{
	// TODO: Done
	frameSize = fsize;
	tableSize = msize / fsize; // Number of Frames = Number of Pages
	*table = calloc(tableSize, sizeof(PROC));
}

/*
 * translate a logical address <pid, page, offset> into a physical address
 * if there is no entry for the pid and the page number in the table, it has to be added
 * if there is no room in the table for the new entry, then the oldest entry has to be removed
 */
long translate(PROC* table, int pid, int page, int offset)
{
	// TODO: Done
	int index = lookUp(table, pid, page);
	if (0 > index)
	{
		struct timespec time;
		clock_gettime(CLOCK_REALTIME, &time);
		index = findOldest(table);
		table[index].pid = pid;
		table[index].page = page;
		table[index].timeStamp = time.tv_sec * 1000000000 + time.tv_nsec;
	}
	return index * frameSize * offset;
}

//
// find the entry for a given pid and page
//
int lookUp(PROC* table, int pid, int page)
{
	// TODO: Done
	for (int i = 0; i < tableSize; ++i)
		if (pid == table[i].pid && page == table[i].page)
			return i;
	return -1;
}

//
// find the oldest entry (or not used that has the timestamp == 0)
//
int findOldest(PROC* table)
{
	// TODO: Done
	int oldestPid = 0;
	time_t oldestTimeStamp = table->timeStamp;
	for (int pid = 0; pid < tableSize; ++pid)
	{
		if (0 == oldestTimeStamp)
			return pid;
		if (oldestTimeStamp > table[pid].timeStamp)
		{
			oldestTimeStamp = table[pid].timeStamp;
			oldestPid = pid;
		}
	}
	return oldestPid;
}

/***
 * prints the inverted table
*/
void printInverted(PROC* table)
{
	printf("-------------------------------------\n");
	printf("        INVERTED TABLE CONTENT\n");
	printf("-------------------------------------\n");
	printf("| PID | PAGE |       TIMESTAMP\n");
	printf("-------------------------------------\n");
	for (int i = 0; i < tableSize; i++)
		printf("| %3d | %4d | %20ld |\n", table[i].pid, table[i].page, table[i].timeStamp);
}
