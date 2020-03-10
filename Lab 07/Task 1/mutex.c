#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

#define oops(ermsg) { perror(ermsg); exit(errno); }

pthread_mutex_t mutex;
pthread_t *threads;

// shared resource
int randomSleepTime;

int maxIterations;
int sleepCeiling;

void *bee(void *param)
{
  int sleepLength;

  int numberOfIterations = 1 + random() % maxIterations;
  while (numberOfIterations-- > 0)
  {
    // try to enter the crituca section
    if (pthread_mutex_lock(&mutex) < 0)
    oops("Mutex lock");

    // in critical section

    randomSleepTime = (randomSleepTime + random() % sleepCeiling) % sleepCeiling;
    printf("This %ld bee is bzziing for %d seconds :-)\n", (long) pthread_self(), randomSleepTime);
    fflush(stdout);

    sleepLength = randomSleepTime;

    // exit the critical section
    if (pthread_mutex_unlock(&mutex) < 0)
    oops("Mutex unlock");

    // out of critical section

    sleep(sleepLength);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    fprintf(stderr, "Usage: mutex numberOfBees maxIterations sleepCeiling\n");
    exit(EXIT_FAILURE);
  }

  // length of the simulation in seconds
  int numberOfBees = strtol(argv[1], NULL, 10);

  // max iterations in bees
  maxIterations = strtol(argv[2], NULL, 10);

  // max sleep length for bees
  sleepCeiling = strtol(argv[3], NULL, 10);

  pthread_mutexattr_t mutex_attr;
#ifdef USE_ADAPTIVE
  pthread_mutexattr_init (&mutex_attr);
  pthread_mutexattr_settype (&mutex_attr, PTHREAD_MUTEX_ADAPTIVE_NP);
#endif

  // initialize the semaphore
  if (pthread_mutex_init(&mutex, &mutex_attr) < 0)
  oops("Mutex initialization");

  // create a bunch of joinable bees
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  threads = (pthread_t *) calloc(numberOfBees, sizeof(pthread_t));

  for (int i = 0; i < numberOfBees; i++)
  {
    if (pthread_create(threads + i, &attr, bee, NULL) < 0)
    oops("Thread creation");
    printf("Bee %ld created\n", (long) (threads + i));
  }

  // wait for the bees
  for (int i = 0; i < numberOfBees; i++)
  {
    if (pthread_join(threads[i], NULL) < 0)
    oops("Thread join");
  }

  pthread_mutex_destroy(&mutex);
  pthread_attr_destroy(&attr);

  exit(EXIT_SUCCESS);
}


