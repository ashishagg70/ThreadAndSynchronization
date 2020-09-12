#include<stdio.h>
#include<stdlib.h>


/* 
 * This is the counter value which is to
 * be incremented by all the threads created 
 * by your program
*/
int counter = 0;



int main(int argc, char const *argv[])
{
	/* Write you code to create n threads here*/
	/* Each thread must increment the counter once and exit*/


	/* Make sure you reap all threads created by your program
	 before printing the counter*/
	printf("%d\n", counter);
	sleep(10000);
	return 0;
}