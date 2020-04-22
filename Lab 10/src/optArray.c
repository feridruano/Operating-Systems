/**
* Name: Ferid Ruano
* Lab/task: Lab 10
* Date: 04/19/2020
**/

/**
 * This implements the LRU page-replacement algorithm.
 */

#include "../inc/optArray.h"

int* referenceString;
int refStringLength;

// the page frame list
int* pageTable;
int numOfFramesPerProcess;

int victimIndex;
int hitPageNumber;

// statistics
int numOfFaults;

// this index is used to go through the sequence of pages references
int currentPgeReferenceIndex;

int testOPT(int numOfFrames, int* refString, int refStrLen)
{
	// TODO: Done
	// Initialize Variables
	referenceString = refString;
	refStringLength = refStrLen;
	numOfFramesPerProcess = numOfFrames;
	victimIndex = FREE_SLOT;
	hitPageNumber = FREE_SLOT;
	pageTable = calloc(numOfFrames, sizeof(int));

	// Set Free Slots in pageTable Array
	for (int frame = 0; frame < numOfFrames; ++frame)
		pageTable[frame] = FREE_SLOT;

	// Iterate Through Reference String
	currentPgeReferenceIndex = 0;
	while (currentPgeReferenceIndex < refStringLength)
	{
		insertOPT(referenceString[currentPgeReferenceIndex]);
		displayOPT();
		currentPgeReferenceIndex++;
	}

	// Clean Up
	freePageTableOPT();
	return numOfFaults;
}

/*
 *	try to insert a page into the page table
 */
void insertOPT(int pageNumber)
{
	int searchVal = searchOPT(pageNumber);

	pageTable[searchVal] = pageNumber;
}

/*
 *  find either an empty slot, or the page, or a vitim to evict
 */
int searchOPT(int pageNumber)
{
	// TODO: Done
	// Iterate through pageTable Array for pageNumber
	for (int frame = 0; frame < numOfFramesPerProcess; ++frame)
	{
		// Empty Frame with pageNumber Found
		if (FREE_SLOT == pageTable[frame])
		{
			numOfFaults++;
			victimIndex = frame;
			return victimIndex;
		}

		// Frame with pageNumber Found
		if (pageNumber == pageTable[frame])
		{
			hitPageNumber = frame;
			return hitPageNumber;
		}
	}
	numOfFaults++;
	return findVictimPageOPT();
}

int findVictimPageOPT()
{
	// TODO: Test
	// Task: Replace the page that will not be used for the longest period of time.
	int found;
	int longestUnusedTime = 0;
	for (int frame = 0; frame < numOfFramesPerProcess; ++frame)
	{
		found = -1;
		// Iterate Through Reference String for Longest Unused Period of Time of a Page.
		for (int refStrIndex = currentPgeReferenceIndex; refStrIndex < refStringLength; ++refStrIndex)
		{
			// Found Frame pageNumber in Reference String Again
			if (pageTable[frame] == referenceString[refStrIndex])
			{
				found = 1;
				// Check for Unused victimIndex or Longer Unused Period of Time
				if (FREE_SLOT == victimIndex || longestUnusedTime < refStrIndex - currentPgeReferenceIndex)
				{
					victimIndex = frame;
					longestUnusedTime = refStrIndex - currentPgeReferenceIndex;
				}
				break;
			}
		}
		if (-1 == found)
			return frame;
	}
	return victimIndex;
}

void displayOPT()
{
	// TODO: Test
	printf("%d ->", referenceString[currentPgeReferenceIndex]);
	for (int frame = 0; frame < numOfFramesPerProcess; ++frame)
	{
		printf("\t%d", pageTable[frame]);
		if (hitPageNumber == frame)
			printf("%c", '<');
//		else if (FREE_SLOT == hitPageNumber) // Fault
//			printf("%c", '*');
	}
	printf("%c", '\n');
}

void freePageTableOPT()
{
	// TODO: Done
	free(pageTable);
}
