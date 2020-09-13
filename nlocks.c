#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
void * incrementDataArray(void * index);

/* 
 * This is the data array which is to
 * be incremented by all the threads created 
 * by your program as specified in the problem state
*/
int data[10];
pthread_mutex_t mutex[10];


int main(int argc, char const *argv[])
{
	/* Write you code to create 10 threads here*/
	pthread_t *threadHandles = malloc(10*sizeof(*threadHandles));
	for(int i=0;i<10;i++){
		int *id=malloc(sizeof(*id));
		*id=i;
		pthread_create(&threadHandles[i],NULL, incrementDataArray, (void *)id);
		pthread_mutex_init(&mutex[i], NULL);
	}
	/* Increment the data array as specified in the problem statement*/
	for(int j=0;j<1000;j++)
	{
		for(int i=0;i<10;i++){
			pthread_mutex_lock(&mutex[i]);
			data[i]++;
			pthread_mutex_unlock(&mutex[i]);
		}
	}
	for(int i=0; i < 10; i++){	
		pthread_join(threadHandles[i], NULL);
	}
	free(threadHandles);
	for(int i=0;i<10;i++){
		pthread_mutex_destroy(&mutex[i]);
	}
	/* Make sure you reap all threads created by your program
	 before printing the counter*/
	for (int i = 0; i < 10; ++i)
	{
		printf("%d\n", data[i]);
	}
	
	sleep(10000);
	return 0;
}


void * incrementDataArray(void* index){
	int rank=*((int*)index);
	for(int i=0;i<1000;i++){
		pthread_mutex_lock(&mutex[rank]);
		data[rank]++;
		pthread_mutex_unlock(&mutex[rank]);
	}
	free(index);
	return NULL;
}