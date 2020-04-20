/**
* Name: Ferid Ruano
* Lab/task: Lab 10
* Date: 04/19/2020
**/

/**
 * This implements the LRU page-replacement algorithm.
 */

#include "../inc/optArray.h"

int *referenceString;
int refStringLength;

// the page frame list
int *pageTable;
int numOfFramesPerProcess;

int victimIndex;
int hitPageNumber;

// statistics
int numOfFaults;

// this index is used to go through the sequence of pages references
int currentPgeReferenceIndex;

int testOPT(int numOfFrames, int *refString, int refStrLen)
{
    // TODO: implement

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
    // TODO: implement

    return findVictimPageOPT();
}

int findVictimPageOPT()
{
    // TODO: implement
    return 0;
}

void displayOPT()
{
    // todo: implement
}

void freePageTableOPT()
{
    // TODO: implement
}
