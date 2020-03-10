#include "buffer.h"

// a buffer to hold items
BUFFER_ITEM *buffer;
int bufferSize;

// a lock to protect the the buffer
pthread_mutex_t mutex;

// counting semaphores
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

int main(int argc, char *argv[])
{
   if (argc < 5)
   {
      fprintf(stderr, "Usage: buffer simulationLength producerThreads consumerThreads bufferSize\n");
      exit(EXIT_FAILURE);
   }

   // length of the simulation in seconds
   int simulationLength = strtol(argv[1], NULL, 10);

   // number of threads
   int producerThreads = strtol(argv[2], NULL, 10);
   int consumerThreads = strtol(argv[3], NULL, 10);

   // size of the buffer
   bufferSize = strtol(argv[4], NULL, 10);
   buffer = malloc(bufferSize * sizeof(BUFFER_ITEM));

   // configure the threads to be joinable
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   // initialize the the critical section lock
   pthread_mutex_init(&mutex, NULL);

   // initialize the counting semphors
   sem_init(&empty, 0, bufferSize);
   sem_init(&full, 0, 0);

   srandom(time(0));

   // create the producer and consumer threads

   pthread_t tid;

   while (producerThreads-- > 0)
      pthread_create(&tid, &attr, producer, NULL);

   while (consumerThreads-- > 0)
   {
      pthread_attr_t attr;
      pthread_attr_init(&attr);
      pthread_create(&tid, &attr, consumer, NULL);
   }

   // sleep for user specified time
   sleep(simulationLength);

   exit(EXIT_SUCCESS);
}

void *producer(void *param)
{
   BUFFER_ITEM rand;

   while (true)
   {
      sleep(random() % bufferSize);

      produce((rand = random()));

      printf("Producer produced %d \n", rand);
   }
}

void *consumer(void *param)
{
   BUFFER_ITEM rand;

   while (true)
   {
      sleep(random() % bufferSize);

      consume(&rand);

      printf("Consumer consumed %d \n", rand);
   }
}

void produce(BUFFER_ITEM item)
{
   // wait if the buffer is empty
   sem_wait(&empty);

   // enter critical section
   pthread_mutex_lock(&mutex);

   buffer[insertPointer++] = item;
   insertPointer = insertPointer % bufferSize;

   // exit critical section
   pthread_mutex_unlock(&mutex);

   // signal the removal of an item from the buffer
   sem_post(&full);
}

void consume(BUFFER_ITEM *item)
{
   // wait if the buffer is full
   sem_wait(&full);

   // exter critical section
   pthread_mutex_lock(&mutex);

   *item = buffer[removePointer];
   buffer[removePointer++] = EMPTY_SLOT;
   removePointer = removePointer % bufferSize;

   // exit critical section
   pthread_mutex_unlock(&mutex);

   // signal the addition to the buffer
   sem_post(&empty);
}

