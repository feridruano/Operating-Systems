#ifndef __buffer_h_
#define __buffer_h_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

typedef int BUFFER_ITEM;

#define EMPTY_SLOT -1

void *producer(void *);

void *consumer(void *);

void produce(BUFFER_ITEM);

void consume(BUFFER_ITEM *);

#endif