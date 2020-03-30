/**
* Name: Ferid Ruano
* Lab/task: Lab 07
* Date: 03/13/2020
**/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define SLEEP_TIME ( (rand() % 5 + 1) * 1000)

// Global Variables
pthread_mutex_t monitor_mutex = PTHREAD_MUTEX_INITIALIZER; // Static Mutex
int numOfSeats, numOfTurns;
int* state;
pthread_cond_t* self;

// Philosopher States
enum
{
	THINKING,
	EATING,
	HUNGRY
};

// Philosopher Eats, If Possible
void test(int id)
{
	if (HUNGRY == state[id] && (EATING != state[(id + 1) % numOfSeats])
		&& (EATING != state[(id - 1) % numOfSeats]))
	{
		state[id] = EATING;
		pthread_cond_signal(&self[id]);
	}
}

// Philosopher Picks Up Chopsticks - Critical Section
void pickup(int id)
{
	pthread_mutex_lock(&monitor_mutex);
	state[id] = HUNGRY;
	test(id);
	if (EATING != state[id])
		pthread_cond_wait(&self[id], &monitor_mutex);
	pthread_mutex_unlock(&monitor_mutex);
}

// Philosopher Puts Down Chopsticks - Critical Section
void putdown(int id)
{
	pthread_mutex_lock(&monitor_mutex);
	state[id] = THINKING;
	test((id + 1) % numOfSeats);
	test((id - 1) % numOfSeats);
	pthread_mutex_unlock(&monitor_mutex);
}

// Dining Philosophers
void* philosopher(void* num)
{
	int id = (int)num;
	printf("Philsopher no. %d sits at the table.\n", id);
	usleep(SLEEP_TIME); // Philosophers arrive at the table at various times

	for (int i = 0; i < numOfTurns; ++i)
	{
		printf("Philosopher no. %d gets hungry for the %d time!\n", id, i + 1);
		pickup(id);
		printf("Philosopher no. %d grabbed chopsticks.\n", id);

		// Generate a random number and use it in a delay in lieu of the eating time.
		// Keeping wait time low, else program can run for a long time.
		sleep(rand() % 5 + 1);
		//printf("Random Number: %d\n", rand() % 5 + 1);

		printf("Philosopher no. %d stopped eating.\n", id);
		putdown(id);
		printf("Philosopher no. %d has returned chopsticks.\n", id);
		printf("Philosopher no. %d finished turn %d.\n", id, i + 1);
	}
	printf(">>>>>> Philosopher no. %d finished meal. <<<<<<\n", id);
	pthread_exit(NULL);
}

// Initialize Philosopher States and Self Conditions
void initialization()
{
	state = calloc(numOfSeats, sizeof(int));
	for (int i = 0; i < numOfSeats; ++i)
		state[i] = THINKING;

	self = calloc(numOfSeats, sizeof(pthread_cond_t)); // Array of PThread Conditions
	for (long i = 0; i < numOfSeats; ++i)
		pthread_cond_init(&self[i], NULL);
}

int main(int argc, char** argv)
{
	// Check for Valid CLI Arguments
	if (argc < 3)
	{
		printf("Usage: philosophers <number of seats> <number of turns>");
		return 1;
	}
	numOfSeats = strtod(argv[1], NULL);
	numOfTurns = strtod(argv[2], NULL);

	// Initialize Environment
	initialization();
	srand(time(NULL)); // Random Seed
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_t philosopher_tid[numOfSeats]; // Philosopher Thread Arrays

	// Create Philosopher Threads
	printf("Start a dinner for %d diners\n", numOfSeats);

	for (long i = 0; i < numOfSeats; ++i)
		pthread_create(&philosopher_tid[i], NULL, philosopher, (void*)i);

	// Join Philosopher Threads After Work is Completed
	for (long j = 0; j < numOfSeats; ++j)
		pthread_join(philosopher_tid[j], NULL);
	printf("Dinner is no more.\n");

	// Destroy Condition Variables
	for (long i = 0; i < numOfSeats; ++i)
		pthread_cond_destroy(&self[i]);

	// Destroy Mutex
	pthread_mutex_destroy(&monitor_mutex);

	// Deallocate Dynamic Memory
	free(state);
	free(self);

	return 0;
}
