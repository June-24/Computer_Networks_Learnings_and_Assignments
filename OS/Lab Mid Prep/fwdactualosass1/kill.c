#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<signal.h>

int main(int argc,char* argv[]){
	int sig=0;
	if(argc>1&&argv[1][0]=='-'){
		int sig=atoi(argv[2]);
		//sig=sig*(-1);
		for(int i=3;i<argc;i++){
			kill(atoi(argv[i]),sig);
			printf("%d",sig);
		}
	}
	else{
		for(int i=1;i<argc;i++){
			kill(atoi(argv[i]),SIGSEGV);
		}
	}
	return 0;
}
