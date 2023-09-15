#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){
	int gpid ;
	printf("Enter GPID Number to send signal : ") ;
	scanf("%d", &gpid) ;
	killpg(gpid, SIGUSR1) ;
}
