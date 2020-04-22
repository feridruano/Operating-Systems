/**
* Name: Ferid Ruano
* Lab/task: Lab 10
* Date: 04/19/2020
**/

/**
 * This implements the LRU page-replacement algorithm.
 */

#include "../inc/lruList.h"

int* referenceString;
int refStringLength;

FRAME* pageTableTop;
FRAME* leastRecentlyUsed;
int pageTableSize;

int numberOfFramesPerProcess = 0;

// statistics
int numberOfFaults = 0;

int hitPageNumber;

/*
 * insert pages from a reference string into a page table and measure
 * the page fault rate
 */
int testLRU(int numOfFrames, int* refString, int refStrLen)
{
	// TODO: Done
	// Initialize Variables
	pageTableSize = 0;
	referenceString = refString;
	refStringLength = refStrLen;
	numberOfFramesPerProcess = numOfFrames;
	// Iterate Through Reference String
	int currentPgeReferenceIndex = 0;
	while (currentPgeReferenceIndex < refStringLength)
	{
		insertLRU(referenceString[currentPgeReferenceIndex]);
		printf("%d ->", referenceString[currentPgeReferenceIndex]);
		displayLRU();
		currentPgeReferenceIndex++;
	}

	// Clean Up
	freePageTableLRU();
	return numberOfFaults;
}

/*
 *	try to insert a page into the page table
 */
void insertLRU(int pageNumber)
{
	// TODO: Test
	FRAME* searchVal = searchLRU(pageNumber);

	// No Frame with pageNumber Found
	if (NULL == searchVal)
	{
		FRAME* newFrame = calloc(1, sizeof(FRAME));
		newFrame->pageNumber = pageNumber;
		numberOfFaults++;
		newFrame.

		// Empty pageTable LL
		if (NULL == pageTableTop)
		{
			pageTableSize++;
			pageTableTop = newFrame;
			leastRecentlyUsed = pageTableTop;
		}
		else
		{
			// Add to Head pageTable LL
			if (pageTableSize < numberOfFramesPerProcess)
			{
				pageTableSize++;
				pageTableTop->up = newFrame;
				newFrame->down = pageTableTop;
				pageTableTop = newFrame;
			}
				// pageTable Full Remove leastRecentlyUsed
			else
			{
				FRAME* newLeastRecentlyUsed = leastRecentlyUsed->up;
				newLeastRecentlyUsed->down = NULL;
				leastRecentlyUsed->up = NULL;
				free(leastRecentlyUsed);
				leastRecentlyUsed = newLeastRecentlyUsed;
				pageTableTop->up = newFrame;
				newFrame->down = pageTableTop;
				pageTableTop = newFrame;
			}
		}
	}
	else
	{
		// Frame with pageNumber Found, Remove Node
		searchVal->up->down = searchVal->down;
		if (leastRecentlyUsed == searchVal)
			leastRecentlyUsed = searchVal->up;
		else
			searchVal->down->up = searchVal->up;

		// Add Frame to Top
		searchVal->up = NULL;
		searchVal->down = pageTableTop;
		pageTableTop->up = searchVal;
		pageTableTop = searchVal;
	}
}

/**
 * Searches for page pageNumber in the page frame list
 * returns NULL if a frame with pageNumber was not found
 * otherwise, returns a reference to the frame with pageNumber
 */
FRAME* searchLRU(int pageNumber)
{
	// TODO: Done
	FRAME* frame = pageTableTop;
	// Traverse through pageTable LL for pageNumber
	while (NULL != frame)
	{
		// Frame with pageNumber Found
		if (pageNumber == frame->pageNumber)
		{
			hitPageNumber = frame->pageNumber;
			return frame;
		}
		frame = frame->down;
	}

	// No Frame with pageNumber Found
	numberOfFaults++;
	hitPageNumber = FREE_SLOT;
	return NULL;
}

void displayLRU()
{
	// TODO: implement
	FRAME* frame = pageTableTop;
	while (NULL != frame)
	{
		printf("\t%d", frame->pageNumber);
		if (hitPageNumber == frame->pageNumber)
			printf("%c", '<');
//		else if (FREE_SLOT == hitPageNumber) // Fault
//			printf("%c", '*');
		frame = frame->down;
	}
	printf("%c", '\n');
}

void freePageTableLRU()
{
	// TODO: Done
	while (NULL != pageTableTop->down)
	{
		pageTableTop = pageTableTop->down;
		free(pageTableTop->up);
	}
	free(pageTableTop);
}

