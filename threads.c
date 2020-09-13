#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>

void * increment();
/* 
 * This is the counter value which is to
 * be incremented by all the threads created 
 * by your program
*/
int counter = 0;
pthread_mutex_t mutex;


int main(int argc, char const *argv[])
{
	/* Write you code to create n threads here*/
	/* Each thread must increment the counter once and exit*/
	long long threadCount;

	
	if(argc ==2){
		threadCount=strtoll(argv[1], NULL, 10);
		pthread_mutex_init(&mutex, NULL);
		pthread_t *threadHandles = malloc(threadCount*sizeof(pthread_t));
		for(long long i=0;i<threadCount;i++){
			pthread_create(&threadHandles[i],NULL, increment, NULL);
		}
		for(long long i=0; i < threadCount; i++){
			pthread_join(threadHandles[i], NULL);
		}
		free(threadHandles);
		pthread_mutex_destroy(&mutex);
	}
	
	/* Make sure you reap all threads created by your program
	 before printing the counter*/
	printf("%d\n", counter);
	sleep(10000);
	return 0;
}

void* increment(){
	pthread_mutex_lock(&mutex);
	counter++;
	pthread_mutex_unlock(&mutex);
	return NULL;
}