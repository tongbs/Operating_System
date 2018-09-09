#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
    pid_t pid ;
    int forkk = 1;
    int i;
    int status ;
    int ret ;
    int a[5] = {0} ;
    printf("Main Process ID : %d\n\n" , getpid()) ;
    pid = fork() ;

	if(pid < 0)
	{
	    printf("Error !!\n") ;
	}
	else if(pid == 0) // children pro
	{
		printf("child1  Fork 1. I'm the child %d, my parent is %d. \n"  , getpid() , getppid()) ;
		if (fork()){
			if (fork()){
				usleep(10000);
			}else{
				if (fork()){
					printf("child2  Fork 2. I'm the child %d, my parent is %d. \n"  , getpid() , getppid()) ;
					usleep(5000);
					wait(0);
				}else{
					usleep(1000);
					printf("child3  Fork 3. I'm the child %d, my parent is %d. \n"  , getpid() , getppid()) ;
				}
			}
		}else{
			if (fork()){
				printf("child4  Fork 2. I'm the child %d, my parent is %d. \n" , getpid() , getppid()) ;
				usleep(5000);
				wait(0);
			}else{
				usleep(1000);
				printf("child5  Fork 3. I'm the child %d, my parent is %d. \n"  , getpid() , getppid()) ;
			}
		}
	}
	else
	{
        usleep(1000) ;
        wait(0) ;
	}
    return 0 ;
}
