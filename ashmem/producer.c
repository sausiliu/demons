#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include "common.h"

int memoryID;
struct wrapper *memory;
int rc;

void atexit_function() {
	rc = shmctl(memoryID, IPC_RMID, NULL);
	rc = shmdt(memory);
	sem_destroy(&memory->pmutex);
	sem_destroy(&memory->cmutex);
	sem_destroy(&memory->empty);
	sem_destroy(&memory->full);
}

int main(int argc, char **argv) {
	atexit(atexit_function);
	//creating key for shared memory
	srand(time(NULL));
	key_t sharedMemoryKey = ftok(".", MEMORY_KEY);
	if (sharedMemoryKey == -1) {
		perror("ftok():");
		exit(1);
	}

	memoryID = shmget(sharedMemoryKey, sizeof(struct wrapper), IPC_CREAT | 0600);
	if (memoryID == -1) {
		perror("shmget():");
		exit(1);
	}

	memory = shmat(memoryID, NULL, 0);
	if (memory == (void *) -1) {
		perror("shmat():");
		exit(1);
	}
	printf("addr memory: %p\n ", memory);
	//initialization
	printf("Initializtaion !\n");
	memset(&memory->array, 0, sizeof(memory->array));
	sem_init(&memory->pmutex, 0, 1);
	sem_init(&memory->cmutex, 0, 1);
	sem_init(&memory->empty, 0, SIZE_OF_ARRAY);
	sem_init(&memory->full, 0, 0);
	memory->n = -1;

	if (memoryID == -1) {
		perror("shmget(): ");
		exit(1);
	}


	while(1)
	{
		int r = rand();
		sem_wait(&memory->empty);
		sem_wait(&memory->pmutex);
		memory->n++;
		(memory->array)[memory->n]=r;

		printf("Adding task\t Value:%d\tNumber of tasks waiting:%d \n",r,memory->n);
		usleep(10000);
		sem_post(&memory->pmutex);
		sem_post(&memory->full);
	}

}
