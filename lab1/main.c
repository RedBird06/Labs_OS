#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void foo()
{
	printf("AT_EXIT\n");
}

int main(int argc,char** argv)
{
	atexit(foo);
	
	pid_t pid;
	int rv;

	switch(pid = fork())
	{
		case -1:
		{
			perror("fork");
			exit(1);
		}
		case 0: 
		{
			printf("<<CHILD>>\n");
			printf("<<CHILD>> CHILD PID -- %d\n",getpid()); 
			printf("<<CHILD>> PARENT PID -- %d\n",getppid()); 
			printf("<<CHILD>> Enter return val (int): ");
			scanf(" %d", &rv);
			printf("<<CHILD>> EXIT %d\n",rv);
			exit(rv);
		}
		default:
		{
			printf("<<PARENT>>\n");
			printf("<<PARENT>> PID -- %d\n",getpid());
			printf("<<PARENT>> CHILD PID -- %d\n",pid);
			printf("<<PARENT>> WAITING FOR <<CHILD>> EXIT\n");
			wait(&rv);
			printf("<<PARENT>> CHILD return val: %d\n",WEXITSTATUS(rv));
			printf("<<PARENT>> EXIT\n");
		}

	}

	return 0;
}
