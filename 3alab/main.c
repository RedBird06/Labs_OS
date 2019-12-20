#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
/*
		
*/
int main ()
{
	
	int pipes[2];
	if (-1 == pipe(pipes))
	{
		perror("PIPE ERR\n");
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
			char temp[100];
			sleep(1);
			time_t tm = time (0);
			close (pipes[1]);
			read(pipes[0],&temp, 100*sizeof(char));
			close (pipes[0]);
			printf ("CHILD TIME : %sRecieved msg: %s",ctime(&tm), temp);
			break;
		}
		default:
		{
			char temp[100];
			time_t tm = time (0);
			sprintf(temp,"%d",getpid());
			strcat(temp," <- PARENT pid | PARENT TIME-> ");
			strcat(temp,ctime(&tm));
			
			close (pipes[0]);
			write (pipes[1], &temp, 100*sizeof(char));
			close (pipes[1]);
			
			sleep(2);
			break;
		}
	}
	return 0;
}