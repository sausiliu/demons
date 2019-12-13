#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define TEXT_SZ 1024

typedef struct {
	sem_t sem_r;
	sem_t sem_w;
	int written;
	char text[TEXT_SZ];
}shared_st;

int main(int argc, const char *argv[])
{
	shared_st * share_mem;
	int shmid;
	char buffer[TEXT_SZ + 1];

	shmid = shmget((key_t)12345, sizeof(shared_st), 0666 | IPC_CREAT);

	share_mem = (shared_st *) shmat(shmid, 0, 0);

	share_mem->written = 0;

	while(1) {
		sem_wait(&share_mem->sem_w);
		fgets(buffer, TEXT_SZ, stdin);
		strncmp((const char *)share_mem->text, buffer, TEXT_SZ);
		share_mem->written = 1;
		if (strncmp(buffer, "end", 3) == 0) break;
		sem_post(&share_mem->sem_r);
	}

	if (shmdt(share_mem) == -1) {}
	sleep(2);
	//if (shmctl(shmid, IPC_RMID, 0) == -1) {}

	return 0;
}

