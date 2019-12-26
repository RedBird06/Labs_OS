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

struct sembuf sem_lock = {0,-1,0}, sem_open = {0,1,0};

int main(){

	key_t key = ftok(FILE, 1);
	int smid = shmget(key, 32, IPC_CREAT|0666);
	if (smid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}
	int semid = semget(key, 1, IPC_CREAT|0666);
	if (semid == -1){
		printf ("SEMAPHORE CREATE FAILED\n");
		return 0;
	}
	char* addr = shmat(smid, NULL, 0);
	if (addr == (char*)-1)
	{
		perror("SHMAT FAILED\n");
		return 0;
	}
	semop(semid,&sem_open,1);
	for(int i = 0;i<25;++i)
	{
		sleep(1);
		semop(semid,&sem_lock,1);
		time_t tt = time(0);
		sprintf(addr, "pid: %d time: %s",getpid() ,ctime(&tt));
		sleep(2);
		semop(semid,&sem_open,1);
	}
	sprintf(addr,"end");
	semctl(smid,0,IPC_RMID);
	shmdt(addr);
	shmctl(smid,IPC_RMID,NULL);

	return 0;
}
