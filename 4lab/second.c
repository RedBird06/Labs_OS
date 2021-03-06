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
        int smid = shmget(key, 32, 0666);
        if (smid == -1)
        {
                perror("SHMEM CREATE FAILED\n");
                return 0;
        }

        char* addr = shmat(smid, NULL, 0);
        if (addr == (char*)-1){
                perror("SHMAT FAILED\n");
                return 0;
        }

        while(1)
	{	
		if(strcmp("end",addr)==0) break;
		sleep (1);
		printf("%s", addr);
	}
	shmdt(addr);

        return 0;
}
