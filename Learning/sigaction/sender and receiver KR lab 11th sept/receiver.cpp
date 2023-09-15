#include<bits/stdc++.h>
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <unistd.h>
#include <signal.h>
using namespace std;
struct sigaction act;
		
void handler(int sig, siginfo_t *siginfo, void *context)
{
	printf("Signal Received!!!!!!\n\n");
    printf("Signal received from process with PID: %d\n",siginfo->si_pid);
    printf("Signal received from process with UID: %d\n",siginfo->si_uid);
    printf("Process P2 with PID: %d ended!\n",getpid());
}

int main()
{
	printf("Process P2 with PID: %d started!\n",getpid());
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;

	int k = sigaction(SIGUSR1 , &act , NULL);
	sleep(1);
	if(k < 0)
	{
		printf("Error in SIGUSR1\n");
		return 1;
	}
	else
	{
		printf("Signal received.\n");
	}
	while(1);
	return 0;
}