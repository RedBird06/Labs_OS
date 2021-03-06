#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sem.h>

#define FILE "shmemochka"
#define buffer_size 52

struct sembuf sem_lock = {0,-1,0}, sem_open = {0,1,0};

int main(){
        
	key_t key = ftok(FILE, 1);
	int shmid = shmget(key, buffer_size, 0666);
	if (shmid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}
	int semid = semget(key, 1, IPC_CREAT|0666);
	if (semid == -1)
	{
		printf ("SEMAPHORE CREATE FAILED\n");
		return 0;
	}
	char* addr = shmat(shmid, NULL, 0);
	if (addr == (char*)-1)
	{
		perror("SHMAT FAILED\n");
		return 0;
	}

	while(1)
	{
		semop(semid, &sem_lock, 1);
		if(strcmp("end",addr)==0) break;
		printf("%s", addr);
		semop(semid, &sem_open, 1);
		sleep(1);
	}
	
	shmdt(addr);

	return 0;
}
