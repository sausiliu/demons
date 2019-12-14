#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>

#define TEXT_SZ 1024

typedef struct {
	char text[TEXT_SZ];
	sem_t sem_r;
	sem_t sem_w;
	int written;
}shared_st;

sem_t test;

int main(int argc, const char *argv[])
{
	shared_st * share_mem;
	int shmid;

	//shmid = shmget((key_t)12345, sizeof(shared_st), 0666 | IPC_CREAT);
	shmid = shmget((key_t)23451, sizeof(shared_st), 0666 | IPC_CREAT);

	printf("line: %d\n", __LINE__);

	if (-1 == shmid) {
		perror("shmget() ");
		exit(1);
	}

	share_mem = (shared_st *) shmat(shmid, 0, 0);

	sem_init(&test, 0, 0);
	printf("addr share_mem: %p\n ", share_mem);
	printf("addr sem_r: %p ,sem_w: %p\n ", &share_mem->sem_r, &share_mem->sem_w);
	sem_init(&share_mem->sem_r, 0, 0);
	sem_init(&share_mem->sem_w, 0, 1);
	printf("line: %d\n", __LINE__);

	share_mem->written = 0;

	while(1) {
		sem_wait(&share_mem->sem_r);
		printf("Wrote: %s", share_mem->text);
		if(strncmp((const char *)share_mem->text, "end", 3) == 0) break;
		sem_post(&share_mem->sem_w);
	}

	sem_post(&share_mem->sem_w);
	if (shmdt(share_mem) == -1) {}
	if (shmctl(shmid, IPC_RMID, 0) == -1) {}

	return 0;
}

