/* Basic Pthreads example using mutex for variable manipulation
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int sum = 0;
sem_t mutex;

void *ThreadOne(void *argument)
{
	sem_wait(&mutex);
	for(int i = 1; i <= 10; i++) sum += i;
	sem_post(&mutex);
	return NULL;
}

void *ThreadTwo(void *argument)
{
	sem_wait(&mutex);
	for(int i = 11; i <= 20; i++) sum += i;
	sem_post(&mutex);
	return NULL;
}

void *ThreadThree(void *argument)
{
	sem_wait(&mutex);
	sum *= 10;
	sem_post(&mutex);
	return NULL;
}
 
int main(void)
{
	sem_init(&mutex, 0, 1);
	pthread_t thr1, thr2, thr3;
	int rc;	 

	// create the threads
	rc = pthread_create(&thr1, NULL, ThreadOne, (void *) NULL);
	rc = pthread_create(&thr2, NULL, ThreadTwo, (void *) NULL);
	rc = pthread_create(&thr3, NULL, ThreadThree, (void *) NULL);

	// wait for them to return
	rc = pthread_join(thr1, NULL);
	rc = pthread_join(thr2, NULL);
	rc = pthread_join(thr3, NULL);

	printf("The total is: %d\n", sum);
 	
	// clean up
	sem_destroy(&mutex);
	exit(EXIT_SUCCESS);
}
