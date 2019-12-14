#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include <sys/shm.h>

int memoryID;
struct wrapper *memory;
int check_prime(int a);
int main(int argc, char **argv) {
	srand(time(NULL));
	key_t sharedMemoryKey = ftok(".",MEMORY_KEY);
	if(sharedMemoryKey==-1)
	{
		perror("ftok():");
		exit(1);
	}
	memoryID=shmget(sharedMemoryKey,sizeof(struct wrapper),0);

	if(memoryID==-1)
	{
		perror("shmget(): ");
		exit(1);
	}

	memory = shmat(memoryID,NULL,0);
	if(memory== (void*)-1)
	{
		perror("shmat():");
		exit(1);
	}

	while(1)
	{
		sem_wait(&memory->full);
		sem_wait(&memory->cmutex);

		int n = memory->n;
		int temp = (memory->array)[n];
		printf("Removed item: %d\tPrime:%d\tNumer of tasks left:%d\n",
				temp, check_prime(temp),n);
		memory->n--;
		usleep(10000);

		sem_post(&memory->cmutex);
		sem_post(&memory->empty);
	}

}
