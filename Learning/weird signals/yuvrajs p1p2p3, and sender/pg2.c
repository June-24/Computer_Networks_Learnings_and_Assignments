#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void handler_SIGUSR1(int signo){
	write(1, "\nProcess2 recieved a SIGUSR1 signal!!!\n", sizeof "Process2 recieved a SIGUSR1 signal!!!\n") ;
}

int main(int argc, char **argv){
	signal(SIGUSR1, handler_SIGUSR1) ;
	
	printf("Process2 Process PID:\t%d\n", (int)getpid()) ;
	
	int pgid ;
	printf("Enter PGID for Process2 : ") ;
	scanf("%d", &pgid) ;
	getchar() ;
	setpgid(getpid(), pgid) ;
	
	
	printf("Process2 Process GID:\t%d\n", (int)getpgid(getpid())) ;
	sleep(2) ;
	while(1){
		sleep(3) ;
		printf("\nProcess2 process working\tPID:\t%d\tGPID:\t%d\n", (int)getpid()
		   , (int)getpgid(getpid())) ;
	}
	
}
