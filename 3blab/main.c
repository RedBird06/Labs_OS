#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define FIFO_FILE "fifosha"

int main (){
	
	unlink(FIFO_FILE);
	int fifo = mkfifo(FIFO_FILE, 0777);
	if (fifo == -1){
		perror("FIFO CREATE ERR\n");
		return 0;
	}

	pid_t pid;
	switch (pid = fork())
	{
		case -1: 
			perror("FORK ERR\n");
			break;
		case 0: 
		{
			int fifo_read = open(FIFO_FILE, O_RDONLY);
			if(fifo_read == -1){
				perror("FIFO READ ERR\n");
				return 0;
			}
			char *buf = (char*)calloc(100, sizeof(char));
			read(fifo_read, buf, 100);
			close(fifo_read);
			sleep(1);
			time_t tm = time (0);
			printf("CHILD TIME: %sReceived msg: ",ctime(&tm));
			printf("%s",buf);
			break;
		}
		default:
		{
			int fifo_write = open(FIFO_FILE, O_WRONLY);
			if (fifo_write == -1)
			{
				perror("FIFO WRITE ERR\n");
				return 0;
			}
			char temp[100];
			time_t tm = time (0);
			sprintf(temp,"%d",getpid());
			strcat(temp," <- PARENT pid | PARENT TIME-> ");
			strcat(temp,ctime(&tm));
			write(fifo_write, temp, 100);
			close(fifo_write);
			sleep(1);
			break;
		}
	}
	
	return 0;
}