#include<bits/stdc++.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h> 
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
using namespace std;
int main(){

    cout<<"sender started: "<<endl;
    cout<<"Pid of the sender: "<<getpid()<<endl;
    int child_pid = fork();

	if(child_pid > 0){
        sleep(3);
        // sending the pid to the receiver by exec
		kill( child_pid , SIGUSR1);
	}
	else{
		char *args[]={"./receiver",NULL}; 
        execvp(args[0],args);
	}
	return 0;
}