#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/shm.h>

#define FILE "shmemochka"

pthread_rwlock_t lock;

void* thread_func()
{
	key_t key = ftok(FILE, 1);
	int smid = shmget(key, 32, 0666);
	if (smid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}
	char* addr = shmat(smid, NULL, 0);
	if (addr == (char*)-1){
		printf ("SHMAT FAILED\n");
		return 0;
	}
	sleep(1);
	while (1){
        pthread_rwlock_wrlock(&lock);
		printf("TID: %ld Say:%s",pthread_self(),addr);
        pthread_rwlock_unlock(&lock);;
		sleep (1);
	}
	return 0;
}

int main()
{
	
	pthread_t pth[10];
	pthread_rwlock_init(&lock,NULL);
	
	int i;
	int counter;
	
	key_t key = ftok(FILE, 1);
	int smid = shmget(key, 32, IPC_CREAT|0666);
	if (smid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}
	
	char* addr = shmat(smid, NULL, 0);
	if (addr == (char*)-1){
		printf ("SHMAT FAILED\n");
		return 0;
	}
	
	for(i=0;i<10;++i)
	{
		int stat = pthread_create(&pth[i], NULL, thread_func, NULL);
		if (stat != 0){
			printf ("%d thread create fialed\n", stat);
			return 0;
		}
	}
	
	for(counter=0;counter<12;counter++)
	{
		sleep(1);
		pthread_rwlock_rdlock(&lock);
		sprintf(addr, "%d\n", counter);
		pthread_rwlock_unlock(&lock);
	}

	pthread_rwlock_destroy(&lock);
	shmdt(addr);
	shmctl(smid,IPC_RMID,NULL);
	
	return 0;
}