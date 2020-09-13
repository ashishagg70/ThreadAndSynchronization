#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barrier.h"

void barrier_init(struct barrier_t * barrier, int nthreads)
{
	barrier->n=nthreads;
	barrier->threadReachedBarrier=0;
	barrier->threadLeftBarrier=0;
	cond_init(&(barrier->cv1));
	cond_init(&(barrier->cv2));
	lock_init(&(barrier->mutex));
	return;
}

void barrier_wait(struct barrier_t *barrier)
{

	//phsae-1
	lock_acquire(&(barrier->mutex));
	barrier->threadReachedBarrier++;
	if(barrier->threadReachedBarrier==barrier->n){
		barrier->threadLeftBarrier=0;
		cond_broadcast(&(barrier->cv1), &(barrier->mutex));	
	}
	else
	{
		while(barrier->threadReachedBarrier!=barrier->n)
			cond_wait(&(barrier->cv1), &(barrier->mutex));
		
	}
	lock_release(&(barrier->mutex));


	//phase-2
	lock_acquire(&(barrier->mutex));
	barrier->threadLeftBarrier++;
	if(barrier->threadLeftBarrier==barrier->n){
		barrier->threadReachedBarrier=0;
		cond_broadcast(&(barrier->cv2), &(barrier->mutex));
	}
	else{
		while(barrier->threadLeftBarrier!=barrier->n)
			cond_wait(&(barrier->cv2), &(barrier->mutex));
	}
	lock_release(&(barrier->mutex));
	
	
	return;
}