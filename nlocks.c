#include<stdio.h>
#include<stdlib.h>


/* 
 * This is the data array which is to
 * be incremented by all the threads created 
 * by your program as specified in the problem state
*/
int data[10];



int main(int argc, char const *argv[])
{
	/* Write you code to create 10 threads here*/
	/* Increment the data array as specified in the problem statement*/


	/* Make sure you reap all threads created by your program
	 before printing the counter*/
	for (int i = 0; i < 10; ++i)
	{
		printf("%d\n", data[i]);
	}
	
	sleep(10000);
	return 0;
}