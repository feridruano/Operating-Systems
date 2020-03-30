/**
* Name: Ferid Ruano
* Lab/task: Lab 07
* Date: 03/13/2020
**/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define SLEEP_TIME ( (rand() % 5 + 1) * 1000)

void* philosopher(void* id);

pthread_mutex_t* chopstick;

int numOfSeats, numOfTurns;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("Usage: philosophers <number of seats> <number of turns>");
		return 1;
	}

	numOfSeats = strtod(argv[1], NULL);
	numOfTurns = strtod(argv[2], NULL);

	chopstick = calloc(numOfSeats, sizeof(pthread_mutex_t));

	// set the see for random number generator
	srand(time(NULL));

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	printf("Start a dinner for %d diners\n", numOfSeats);

	pthread_t philosopher_tid[numOfSeats];

	long i;
	for (i = 0; i < numOfSeats; i++)
		pthread_mutex_init(chopstick + i, NULL);

	for (i = 0; i < numOfSeats; i++)
		pthread_create(&philosopher_tid[i], NULL, philosopher, (void*)i);

	for (i = 0; i < numOfSeats; i++)
		pthread_join(philosopher_tid[i], NULL);

	for (i = 0; i < numOfSeats; i++)
		pthread_mutex_destroy(chopstick + i);

	printf("Dinner is no more.\n");

	return 0;
}

void* philosopher(void* num)
{
	int id = (long)num;

	printf("Philsopher no. %d sits at the table.\n", id);

	// philosophers arrive at the table at various times
	usleep(SLEEP_TIME);

	int i;
	for (i = 0; i < numOfTurns; i++)
	{
		printf("Philosopher no. %d gets hungry for the %d time!\n", id, i + 1);
		// Grab first chopstick and check for the other chopstick
		while (true)
		{
			printf("Philosopher no. %d tries to grab chopstick %d\n", id, id);
			if (pthread_mutex_trylock(&(chopstick[id])) == 0)
				printf("Philosopher no. %d has grabbed chopstick %d\n", id, id);
			else
				printf("Philosopher no. %d could not grabbed chopstick %d\n", id, id);

			printf("Philosopher no. %d tries to grab chopstick %d\n", id, (id + 1) % numOfSeats);
			if (pthread_mutex_trylock(&(chopstick[(id + 1) % numOfSeats])) == 0)
			{
				printf("Philosopher no. %d grabbed chopstick %d\n", id, (id + 1) % numOfSeats);
				printf("Philosopher no. %d eating\n", id);
				printf("Philosopher no. %d stopped eating\n", id);
				pthread_mutex_unlock(&(chopstick[id]));
				printf("Philosopher no. %d has returned chopstick %d\n", id, id);
				pthread_mutex_unlock(&(chopstick[(id + 1) % numOfSeats]));
				printf("Philosopher no. %d has returned chopstick %d\n", id, (id + 1) % numOfSeats);
				printf("Philosopher no. %d finished turn %d\n", id, i + 1);
				break;
			}
			else
			{
				printf("Philosopher no. %d could not grabbed chopstick %d\n", id, (id + 1) % numOfSeats);
				pthread_mutex_unlock(&(chopstick[id])); // Return unused chopstick
				printf("Philosopher no. %d has returned chopstick %d\n", id, id);
			}
			usleep(SLEEP_TIME);
		}
	}

	printf(">>>>>> Philosopher no. %d finished meal. <<<<<<\n", id);

	pthread_exit(NULL);
}
