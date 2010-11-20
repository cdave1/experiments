#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

static int shared = 0;
pthread_mutex_t mutex;
#define RAND_MASK ((1<<12) - 1)

void * Read(void *arg)
{
	int tid = *((int *) arg);
	pthread_mutex_lock(&mutex);
	printf("I'm reader %d, and the shared variable is is: %d\n", tid, shared);
	pthread_mutex_unlock(&mutex);
	return NULL;
}


void * Write(void *arg)
{
        int tid = *((int *) arg);
	pthread_mutex_lock(&mutex);
	int i = 0;
	while (i++ < 1000)
	{
		shared = rand() & RAND_MASK;
	} 
        printf("I'm writer %d, and I just set the shared variable to: %d\n", tid, shared);
        pthread_mutex_unlock(&mutex);
        return NULL;
}


int main(int argc, char ** argv)
{
	pthread_mutex_init(&mutex, NULL);

	pthread_t readers[20];
	pthread_t writers[20];
	int thread_args[20];
	unsigned i;

	for (i = 0; i < 20; ++i)
	{
		thread_args[i] = i;
		pthread_create(&readers[i], NULL, Read, (void *)&thread_args[i]);
	}

	for (i = 0; i < 20; ++i)
	{
		thread_args[i] = i;
		pthread_create(&writers[i], NULL, Write, (void *)&thread_args[i]);
	}

	for (i = 0; i < 20; ++i)
	{
		pthread_join(readers[i], NULL);
	}

	for (i = 0; i < 20; ++i)
	{
		pthread_join(writers[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);

	return 0;
}
