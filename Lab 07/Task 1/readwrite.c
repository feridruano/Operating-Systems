#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>

#define oops(ermsg) { perror(ermsg); exit(errno); }

// protected shared resources
int count = 0;
int notDone = true;

// constant target
int countingTarget;

pthread_t *threads;

int sleepCeiling;

// read write lock
pthread_rwlock_t lock;

void *writer(void *arg)
{
  long myLimit = (long) arg;

  printf("Writer %x started with count limit of %ld.\n", (unsigned int) pthread_self(), myLimit);

  for (int i = 0; (i < myLimit) && notDone; i++)
  {
    if (pthread_rwlock_wrlock(&lock) < 0)
    oops("Read-write lock lock");

    count++;

    notDone = ((count < countingTarget) ? true : false);

    printf("Writer %x: count = %d.\n", (unsigned int) pthread_self(), count);

    if (pthread_rwlock_unlock(&lock) < 0)
    oops("Read-write lock unlock");

    sleep(random() % sleepCeiling);
  }
  pthread_exit(NULL);
}

void *reader(void *arg)
{
  printf("Reader %x started.\n", (unsigned int) pthread_self());

  while (notDone)
  {
    if (pthread_rwlock_rdlock(&lock) < 0)
    oops("Read-write lock lock");

    printf("Reader %x: count = %d.\n", (unsigned int) pthread_self(), count);

    if (pthread_rwlock_unlock(&lock) < 0)
    oops("Read-write lock unlock");

    sleep(random() % sleepCeiling);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc < 6)
  {
    fprintf(stderr, "Usage: readwrite countingTarget numberOfReaders numberOfWriters countCeiling sleepCeiling\n");
    exit(EXIT_FAILURE);
  }

  // length of the simulation in seconds
  countingTarget = strtol(argv[1], NULL, 10);

  // number of threads
  int numberOfReaders = strtol(argv[2], NULL, 10);
  int numberOfWriters = strtol(argv[3], NULL, 10);
  int countCeiling = strtol(argv[4], NULL, 10);
  sleepCeiling = strtol(argv[5], NULL, 10);

  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  if (pthread_rwlock_init(&lock, NULL) < 0)
  oops("Creating a read-write lock");

  // Create the counting threads and the watcher thread

  threads = (pthread_t *) calloc(numberOfWriters + numberOfReaders, sizeof(pthread_t));

  for (int i = 0; i < numberOfReaders; i++)
    if (pthread_create(threads + i, &attr, reader, NULL) < 0)
    oops("Reader thread creation");

  for (int i = 0; i < numberOfWriters; i++)
    if (pthread_create(threads + i, &attr, writer, (void *) (1 + random() % (countCeiling - 1))) < 0)
    oops("Writer thread creation");

  /* Wait for all threads to complete */
  for (int i = 0; i < numberOfWriters + numberOfReaders; i++)
    if (pthread_join(threads[i], NULL) < 0)
    oops("Thread joining");

  printf("Back in main(). Joined on %d threads. Done.\n", numberOfWriters + numberOfReaders);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);

  pthread_rwlock_destroy(&lock);

  exit(EXIT_SUCCESS);
}
