#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/shm.h>

#define FILE "shmemochka"
#define buffer_size 52

pthread_mutex_t mutex;

void* thread_func()
{
	key_t key = ftok(FILE, 1);
	int shmid = shmget(key, buffer_size, 0666);
	if (shmid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}
	char* addr = shmat(shmid, NULL, 0);
	if (addr == (char*)-1)
	{
		printf ("SHMAT FAILED\n");
		return 0;
	}
	while (1)
	{
    pthread_mutex_lock(&mutex);
		printf("TID: %ld Say:%s",pthread_self(),addr);
    pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	return 0;
}

int main()
{
	pthread_t pth[10];
	int i;
	int counter = -1;
	pthread_mutex_init(&mutex, NULL);
	key_t key = ftok(FILE, 1);
	int shmid = shmget(key, buffer_size, IPC_CREAT|0666);
	if (shmid == -1)
	{
		perror("SHMEM CREATE FAILED\n");
		return 0;
	}	
	char* addr = shmat(shmid, NULL, 0);
	if (addr == (char*)-1)
	{
		printf ("SHMAT FAILED\n");
		return 0;
	}

	sprintf(addr, "%d\n", counter);

	for(i=0;i<10;++i)
	{
		int stat = pthread_create(&pth[i], NULL, thread_func, NULL);
		if (stat != 0)
		{
			printf ("%d THREAD CREATE FAILED\n", stat);
			return 0;
		}
	}
	
	for(counter=0;counter<12;counter++)
	{
		sleep(1);
		pthread_mutex_lock(&mutex);
		sprintf(addr, "%d\n", counter);
		pthread_mutex_unlock(&mutex);
	}

	pthread_mutex_destroy(&mutex);
	shmdt(addr);
	shmctl(shmid,IPC_RMID,NULL);
	
	return 0;
}
