#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

//#define USE_SPINLOCK
//#define USE_ADAPTIVE

#ifdef USE_SPINLOCK
pthread_spinlock_t spinlock;
#else
pthread_mutex_t mutex;
#endif

pthread_t *threads;

// shared protected resource
int randomSleepTime;

int maxIterations;
int sleepCeiling;

void *bee(void *param)
{
  int sleepLength;

  int numberOfIterations = 1 + random() % maxIterations;
  while (numberOfIterations-- > 0)
  {
    // try to enter the critical section
#ifdef USE_SPINLOCK
    pthread_spin_lock(&spinlock);
#else
    pthread_mutex_lock(&mutex);
#endif
    // in critical section

    randomSleepTime = (randomSleepTime + random() % sleepCeiling) % sleepCeiling;
    printf("This %ld bee is bzziing for %d seconds :-)\n", (long) pthread_self(), randomSleepTime);
    fflush(stdout);

    sleepLength = randomSleepTime;

    // exit the critical section
#ifdef USE_SPINLOCK
    pthread_spin_unlock(&spinlock);
#else
    pthread_mutex_unlock(&mutex);
#endif
    // out of critical section

    sleep(sleepLength);
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    fprintf(stderr, "Usage: mutex_spinlock numberOfBees maxIterations sleepCeiling\n");
    exit(EXIT_FAILURE);
  }

  // length of the simulation in seconds
  int numberOfBees = strtol(argv[1], NULL, 10);

  // max iterations in bees
  maxIterations = strtol(argv[2], NULL, 10);

  // max sleep length for bees
  sleepCeiling = strtol(argv[3], NULL, 10);

  // initialize the semaphore
#ifdef USE_SPINLOCK
  printf("Using spinlocks...\n");
  pthread_spin_init(&spinlock, 0);
#else
  pthread_mutexattr_t mutex_attr;
#ifdef USE_ADAPTIVE
  printf("Using adaptive...\n");
  pthread_mutexattr_init (&mutex_attr);
  pthread_mutexattr_settype (&mutex_attr, PTHREAD_MUTEX_ADAPTIVE_NP);
#endif
  printf("mutex locks...\n");
  pthread_mutex_init(&mutex, &mutex_attr);
#endif

  pthread_attr_t attr;
  pthread_attr_init(&attr);

  // Create a bunch of bees
  threads = (pthread_t *) calloc(numberOfBees, sizeof(pthread_t));
  for (int i = 0; i < numberOfBees; i++)
  {
    pthread_create(threads + i, &attr, bee, NULL);
    printf("Bee %ld created\n", (long) threads[i]);
  }

  // wait for the bees
  for (int i = 0; i < numberOfBees; i++)
    pthread_join(threads[i], NULL);

#ifdef USE_SPINLOCK
  pthread_spin_destroy(&spinlock);
#else
  pthread_mutex_destroy(&mutex);
#endif
  pthread_attr_destroy(&attr);

  exit(EXIT_SUCCESS);
}


