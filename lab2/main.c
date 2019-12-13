#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

void iwannaseethemonth(int month)
{
	switch(month)
	{
		case 1: printf(" Jan "); break;
		case 2: printf(" Feb "); break;
		case 3: printf(" Mar "); break;
		case 4: printf(" Apr "); break;
		case 5: printf(" May "); break;
		case 6: printf(" Jun "); break;
		case 7: printf(" Jul "); break;
		case 8: printf(" Aug "); break;
		case 9: printf(" Sep "); break;
		case 10: printf(" Oct "); break;
		case 11: printf(" Nov "); break;
		case 12: printf(" Dec "); break;
		default: printf(" BUE "); break;
	}
}

int peeker(char *name)
{
	struct stat filestat;
	struct passwd *pwd;
	struct group *grp;
	struct tm *_time;

	stat(name, &filestat);
	pwd = getpwuid(filestat.st_uid);
	grp = getgrgid(filestat.st_gid);

	
	if(filestat.st_mode & S_IFDIR) printf("d");
	else if(filestat.st_mode & S_IFCHR) printf("s");
	else if(filestat.st_mode & S_IFIFO) printf("p");
	else if(filestat.st_mode & S_IFBLK) printf("b");
	else printf("-");
	if(filestat.st_mode & S_IRUSR) printf("r"); else printf("-");
	if(filestat.st_mode & S_IWUSR) printf("w"); else printf("-");
	if(filestat.st_mode & S_IXUSR) printf("x"); else printf("-");
	if(filestat.st_mode & S_IRGRP) printf("r"); else printf("-");
	if(filestat.st_mode & S_IWGRP) printf("w"); else printf("-");
	if(filestat.st_mode & S_IXGRP) printf("x"); else printf("-");
	if(filestat.st_mode & S_IROTH) printf("r"); else printf("-");
	if(filestat.st_mode & S_IWOTH) printf("w"); else printf("-");
	if(filestat.st_mode & S_IXOTH) printf("x"); else printf("-");

	printf(" %ld",filestat.st_nlink);
	printf(" %s",pwd->pw_name);
	printf(" %s",grp->gr_name);
	printf(" %5d",(int)filestat.st_size);

	time_t f_time = filestat.st_ctime;
	_time = localtime(&f_time);
	iwannaseethemonth(_time->tm_mon+1);

	printf(" %02d", _time->tm_mday);
	printf(" %02d:%02d ", _time->tm_hour, _time->tm_min);
	printf(" %s\n",name);

	return 0;
}

int main(int argc, char** argv)
{

	  struct dirent **namelist;
		int count;

		count = scandir(".", &namelist, 0, alphasort);
		if (count < 0) perror("scandir");
		else 
		{
			printf("%d files in current dir\n",count-2);	
			for(int i = 0;i<count;++i)
			 	{
					peeker(namelist[i]->d_name);
					free(namelist[i]);
		    }
	  	free(namelist);
		}

	return 0;
}
