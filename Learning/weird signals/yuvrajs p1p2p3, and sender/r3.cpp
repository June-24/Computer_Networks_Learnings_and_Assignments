#include<bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
using namespace std;

void handler(int signo){
    cout<<"R3 recieved a signal"<<endl ;
}

int main(){
	signal(SIGINT, handler) ;
	cout<<"R3 process PID: "<<(int)getpid()<<endl ;
	printf("R3 initial Process GID:\t%d\n", (int)getpgid(getpid())) ;

	
	int pgid ;
	printf("Enter new PGID : ") ;
    cin>>pgid ;
	setpgid(getpid(), pgid) ;
	
	
	printf("R3 Process GID:\t%d\n", (int)getpgid(getpid())) ;
    string t="kill -STOP "+to_string(getpid()) ;
    system(t.c_str()) ;
	sleep(2) ;
	while(1){
		sleep(3) ;
		printf("\nR3 process working\tPID:\t%d\tGPID:\t%d\n", (int)getpid()
		   , (int)getpgid(getpid())) ;
	}
}
