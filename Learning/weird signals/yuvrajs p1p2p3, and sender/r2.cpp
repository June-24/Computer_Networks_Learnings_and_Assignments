#include<bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
using namespace std;

void handler(int signo){
    cout<<"R2 recieved a signal"<<endl ;
}

int main(){
	signal(SIGINT, handler) ;
	
	printf("R2 Process PID:\t%d\n", (int)getpid()) ;
	cout<<"initial GID of R2: "<<(int)getpgid(getpid())<<endl ;
	int pgid ;
	printf("Enter PGID for R2 : ") ;
    cin>>pgid ;
	setpgid(getpid(), pgid) ;

	printf("R2 Process GID:\t%d\n", (int)getpgid(getpid())) ;
    string t="kill -STOP "+to_string(getpid()) ;
    system(t.c_str()) ;
	sleep(2) ;
	while(1){
		sleep(3) ;
		printf("\nProcess2 process working\tPID:\t%d\tGPID:\t%d\n", (int)getpid()
		   , (int)getpgid(getpid())) ;
	}
	
}
