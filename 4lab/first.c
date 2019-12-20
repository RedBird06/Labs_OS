#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

#define FILE "shmemochka"

int main(){

	key_t key = ftok(FILE, 1);
	int smid = shmget(key, 32, IPC_CREAT|0666);
	if (smid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}

	char* addr = shmat(smid, NULL, 0);
	if (addr == (char*)-1)
	{
		perror("SHMAT FAILED\n");
		return 0;
	}

	for(int i = 0;i<25;++i)
	{
		time_t tt = time(0);
		sprintf(addr, "pid: %d time: %s",getpid() ,ctime(&tt));
		sleep(1);
	}
	sprintf(addr,"end");
	shmdt(addr);
	shmctl(smid,IPC_RMID,NULL);

	return 0;
}
