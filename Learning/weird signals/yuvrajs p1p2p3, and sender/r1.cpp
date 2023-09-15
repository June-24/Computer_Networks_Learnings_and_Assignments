#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
using namespace std;
void handler_SIGUSR1(int signo){
    cout<<"R1 recieved a signal"<<endl ;
}

int main(){

	printf("R1 Process PID:\t%d\n", (int)getpid()) ;
	printf("R1 Process GID:\t%d\n", (int)getpgrp()) ;
    string t="kill -STOP "+to_string(getpid()) ;
	system(t.c_str()) ;
	signal(SIGINT, handler_SIGUSR1) ;
	sleep(2) ;
	while(1){
		sleep(3) ;
		printf("\nR1 process working\tPID:\t%d\tGPID:\t%d\n", (int)getpid()
		   , (int)getpgid(getpid())) ;
	}
}
