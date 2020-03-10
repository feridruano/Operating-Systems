#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

#define oops(ermsg)    { perror(ermsg); exit(errno); }

// protected shared resources
int count = 0;
bool notDone = true;

// constant target
int countingTarget;

pthread_t *threads;

int sleepCeiling;

pthread_mutex_t countMutex;
pthread_cond_t countTargetConditionVariable;

void *counter(void *arg)
{
  long myCountLimit = (long) arg;

  printf("Counter %x:  started with count limit of %ld.\n", (unsigned int) pthread_self(), myCountLimit);

  int i;
  for (i = 0; (i < myCountLimit) && notDone; i++)
  {
    if (pthread_mutex_lock(&countMutex) < 0)
    oops("Mutex lock");

    count++;

    if (count >= countingTarget)
    {
      printf("Counter %x, count = %d:  threshold reached.\n", (unsigned int) pthread_self(), count);

      if (pthread_cond_signal(&countTargetConditionVariable) < 0)
      oops("Conditional variable signal");

      notDone = false;
    }

    printf("Counter %x, count = %d: unlocking mutex.\n", (unsigned int) pthread_self(), count);

    if (pthread_mutex_unlock(&countMutex) < 0)
    oops("Mutex unlock");

    sleep(random() % sleepCeiling);
  }

  pthread_exit(NULL);
}

void *watcher(void *arg)
{
  printf("Wacther %x: started.\n", (unsigned int) pthread_self());

  if (pthread_mutex_lock(&countMutex) < 0)
  oops("Mutex lock");

  // block here until a signal indicating that the variable has reached
  // the threshold arrives
  printf("Watcher %x: waiting for a signal.\n", (unsigned int) pthread_self());

  // this must be done inside the section protected by the associated mutex
  pthread_cond_wait(&countTargetConditionVariable, &countMutex);

  printf("Watcher %x: condition signal received.\n", (unsigned int) pthread_self());

  if (pthread_mutex_unlock(&countMutex) < 0)
  oops("Mutex unlock");

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc < 5)
  {
    fprintf(stderr, "Usage: condvar countingTarget numberOfCounters countCeiling sleepCeiling\n");
    exit(EXIT_FAILURE);
  }

  // length of the simulation in seconds
  countingTarget = strtol(argv[1], NULL, 10);

  // number of threads
  int numberOfCounters = strtol(argv[2], NULL, 10);
  int countCeiling = strtol(argv[3], NULL, 10);
  sleepCeiling = strtol(argv[4], NULL, 10);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  pthread_mutex_init(&countMutex, NULL);
  pthread_cond_init(&countTargetConditionVariable, NULL);

  // Create the counting threads and the watcher thread

  threads = (pthread_t *) calloc(numberOfCounters + 1, sizeof(pthread_t));

  int i;
  for (i = 0; i < numberOfCounters; i++)
  {
    if (pthread_create(threads + i, &attr, counter, (void *) (random() % countCeiling)) < 0)
    oops("Thread creation");
  }

  pthread_create(threads + i, NULL, watcher, NULL);

  // wait for the watcher
  pthread_join(threads[i], NULL);

  /* Could also wait for all other threads to complete */

  for (i = 0; i < numberOfCounters; i++)
  {
    pthread_join(threads[i], NULL);
  }
  printf("Back in main(). Joined on %d  threads. Done.\n", numberOfCounters + 1);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&countMutex);
  pthread_cond_destroy(&countTargetConditionVariable);

  exit(EXIT_SUCCESS);
}
