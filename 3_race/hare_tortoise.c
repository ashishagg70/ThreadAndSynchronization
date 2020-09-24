#include "cs_thread.h"

struct Repositioning {
	char player; 		// T for turtle and H for hare
	int time; 		// At what time god interrupt's
	int distance;		// How much does god move any of the player. 
							// distance can be negetive or positive.
							// using this distance if any of players position is less than zero then bring him to start line.
							// If more than finish_distance then make him win.
							// If time is after completion of game than you can ignore it as we will already have winner.
};

struct race {
	
	//	Don't change these variables.
	//	speeds are unit distance per unit time.
	int printing_delay;
	int tortoise_speed;					// speed of Turtle
	int hare_speed;						// speed of hare
	int hare_sleep_time; 				// how much time does hare sleep (in case he decides to sleep)
	int hare_turtle_distance_for_sleep; // minimum lead which hare has on turtle after which hare decides to move
										// Any lead greater than this distance and hare will ignore turtle and go to sleep
	int finish_distance;				// Distance between start and finish line
	struct Repositioning* reposition;	// pointer to array containing Randomizer's decision
	int repositioning_count;			// number of elements in array of repositioning structure
	
	//	Add your custom variables here.

	
	
};


void* Turtle(void *race);
void* Hare(void *race);
void* Randomizer(void *race);
void* Report(void *race);

//my variables
int harePositon=0;
int turtlePosition=0;
int winner=-1;
struct condition cvp; //randomizer
struct condition cvh; //hare
struct condition cvt; //turtle
struct condition cvr; // report
int pdone=0;
int hdone=0;
int tdone=0;
int rdone=1;
struct lock mutex1;
//struct lock mutex2;
int globalClock=0;

char init(struct race *race)
{
	pthread_t *threadHandles = malloc(4*sizeof(*threadHandles));
	lock_init(&mutex1);
	//lock_init(&mutex2);
	cond_init(&cvr);
	cond_init(&cvp);
	cond_init(&cvt);
	cond_init(&cvh);
	pthread_create(&threadHandles[0],NULL, Turtle, race);
	pthread_create(&threadHandles[1],NULL, Hare, race);
	pthread_create(&threadHandles[2],NULL, Randomizer, race);
	pthread_create(&threadHandles[3],NULL, Report, race);

	for(long long i=0; i < 4; i++){
			pthread_join(threadHandles[i], NULL);
		}
	return winner;
}

void* Turtle(void *arg)
{
	struct race *race=((struct race*)arg);
	while(winner==-1){
		lock_acquire(&mutex1);
		while(hdone!=1)
			cond_wait(&cvt, &mutex1);
		hdone=0;
		if(winner==-1 || winner=='h'){
			turtlePosition+=race->tortoise_speed;
			if(turtlePosition>=race->finish_distance)
				winner='T';
			else if(winner=='h'){
				winner='H';
			}
		}

		tdone=1;
		cond_signal(&cvr, &mutex1);
		lock_release(&mutex1);
	}
	
	return NULL;
  
}

void* Hare(void *arg)
{
	struct race *race=((struct race*)arg);
	int isSleeping=0;
	int localclock=0;
	while(winner==-1){
		lock_acquire(&mutex1);
		while(pdone!=1)
			cond_wait(&cvh, &mutex1);
		pdone=0;
		if(winner==-1)
		{
			if(localclock==race->hare_sleep_time && isSleeping)
			{
				localclock=0;
				isSleeping=0;
			}
			else if(isSleeping){
				localclock++;
			}
			else if(!isSleeping){
				if((harePositon-turtlePosition)>race->hare_turtle_distance_for_sleep)
				{
					if(race->hare_sleep_time!=0)
						isSleeping=1;
				}
					
				else{
					harePositon+=race->hare_speed;
					if(harePositon>=race->finish_distance)
						winner='h';
					if(race->hare_sleep_time!=0)
						isSleeping=1;
				}

			}
		}
		
		hdone=1;
		cond_signal(&cvt, &mutex1);
		lock_release(&mutex1);

	}
	
	return NULL;
}


void* Randomizer(void *arg)
{

	int i=0;
	struct race *race=((struct race*)arg);
	while(winner==-1){
		
		lock_acquire(&mutex1);
		while(rdone!=1)
			cond_wait(&cvp, &mutex1);
		rdone=0;
		if(winner==-1){
			if(i<race->repositioning_count && race->reposition[i].time==globalClock){
				//printf("i: %d| race->repsiton->time: %d, globalTime: %d\n", i, race->reposition[i].time, globalClock);
				if(race->reposition[i].player=='T'){
					//printf("T\n");
					turtlePosition+=race->reposition[i].distance;
					if(turtlePosition<0)
						turtlePosition=0;
					if(turtlePosition>=race->finish_distance){
						winner='T';
					}
				}else{
					//printf("H\n");
					//printf("HarePositino: %d| change: %d\n", harePositon, race->reposition[i].distance);
					harePositon+=race->reposition[i].distance;
					if(harePositon<0)
						harePositon=0;
					if(harePositon>=race->finish_distance){
						winner='H';
					}
				}
				i++;
			}
		}

		pdone=1;
		cond_signal(&cvh, &mutex1);
		lock_release(&mutex1);

	}
	return NULL;
}

void* Report(void *arg)
{

	struct race *race=((struct race*)arg);
	int localclock=0;
	while(winner==-1){
		lock_acquire(&mutex1);
		while(tdone!=1)
			cond_wait(&cvr, &mutex1);
		tdone=0;
		localclock++;
		if(winner==-1){
			if(race->printing_delay==localclock){
				localclock=0;
				printf("Global Clock: %d unit done| Position of Hare: %d\n",globalClock+1,  harePositon);
				printf("Global Clock: %d unit done| Position of tortoise: %d\n", globalClock+1, turtlePosition);

			}

		}
		globalClock++;
		rdone=1;
		cond_signal(&cvp, &mutex1);
		lock_release(&mutex1);
	}
	
	return NULL;
}

