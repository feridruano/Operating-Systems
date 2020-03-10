#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#define oops(ermsg) { perror(ermsg); exit(errno); }

sem_t semaphore;
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
    // wait
    sem_wait(&semaphore);

    // in critical section

    randomSleepTime = (randomSleepTime + random() % sleepCeiling) % sleepCeiling;
    printf("This %ld bee is bzziing for %d seconds :-)\n", (long) pthread_self(), randomSleepTime);
    fflush(stdout);

    sleepLength = randomSleepTime;

    // signal
    sem_post(&semaphore);

    // out of critical section
    sleep(sleepLength);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    fprintf(stderr, "Usage: semaphore numberOfBees maxIterations sleepCeiling\n");
    exit(EXIT_FAILURE);
  }

  // length of the simulation in seconds
  int numberOfBees = strtol(argv[1], NULL, 10);

  // max iterations in bees
  maxIterations = strtol(argv[2], NULL, 10);

  // max sleep length for bees
  sleepCeiling = strtol(argv[3], NULL, 10);

  // initialize the semaphore
  if (sem_init(&semaphore, 0, 0) < 0)
  oops("Semaphore initialization");

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  // create a bunch of bees
  threads = (pthread_t *) calloc(numberOfBees, sizeof(pthread_t));

  for (int i = 0; i < numberOfBees; i++)
  {
    if (pthread_create(threads + i, &attr, bee, NULL) < 0)
    oops(
        "Semaphore initialization");
    printf("Bee %ld created\n", (long) threads + i);
  }

  // signal
  sem_post(&semaphore);

  // wait for the bees
  for (int i = 0; i < numberOfBees; i++)
  {
    if (pthread_join(threads[i], NULL) < 0)
    oops("Semaphore initialization");
  }
  sem_destroy(&semaphore);
  pthread_attr_destroy(&attr);

  exit(EXIT_SUCCESS);
}
