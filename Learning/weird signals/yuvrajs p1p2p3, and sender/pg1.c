#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

void handler_SIGUSR1(int signo){
	// write is async safe
	write(1, "\nProcess1 recieved a SIGUSR1 signal!!!\n", sizeof "Process1 recieved a SIGUSR1 signal!!!\n") ;
}

int main(int argc, char **argv){

	printf("Process1 Process PID:\t%d\n", (int)getpid()) ;
	printf("Process1 Process GID:\t%d\n", (int)getpgrp()) ;
	
	signal(SIGUSR1, handler_SIGUSR1) ;
	sleep(2) ;
	while(1){
		sleep(3) ;
		printf("\nProcess1 process working\tPID:\t%d\tGPID:\t%d\n", (int)getpid()
		   , (int)getpgid(getpid())) ;
	}
}
