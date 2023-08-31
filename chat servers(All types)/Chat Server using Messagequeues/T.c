#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<poll.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<math.h>
#include<sys/poll.h>

struct message{
	long type;
	char buffer[100];
};

char* itoa(int a){
	char* temp="";
	while(a){
		char x=a%10;
		x=x+'0';
		char y[1]={x};
		strcat(temp,y);
		a=a/10;
	}
	char* actual="";
	for(int i=strlen(temp)-1;i>=0;i--){
		char y[1]={temp[i]};
		strcat(actual,y);
	}
	return actual;
}

int main(){
	int key1=ftok("MSGQ1",65); //reads from here
	int key2=ftok("MSGQ2",67); //writes from here
	printf("keys created\n");
	int msqid1=msgget(key1,0777| IPC_CREAT);
	int msqid2=msgget(key2,0777| IPC_CREAT);
	printf("msggetted\n");
	char x[5];
	printf("which all groups are you in?(eg:123 as in grp1,grp2,grp3): ");
	scanf("%s",x);
	struct pollfd pfds[2];
	pfds[0].fd=0;
	pfds[0].events=POLLIN;
	pfds[1].fd=msqid1;
	pfds[1].events=POLLIN;
	printf("polls created\n");
	int count=1;
	while(count){
		int ret=poll(pfds,2,500);
		if(pfds[0].revents&POLLIN){
			char some[50];
			fgets(some,50,stdin);
			struct message msg;
			//strcpy(msg.buffer,itoa(getpid()));
			printf("%s here\n",some);
			msg.type=some[0]-'0';
			char y[1]={some[2]};
			strcpy(msg.buffer,y);
			for(int i=0;i<strlen(some);i++){
				char z[1]={some[i]};
				strcat(msg.buffer,z);
			}
			strcat(msg.buffer,": ");
			int flag=0;
			for(int i=0;i<strlen(x);i++){
				if((x[i]-'0')==msg.type){
					flag=1;
					break;
				}
			}
			if(flag){
				msgsnd(msqid2,&msg,sizeof(msg),0);
			}
			else{
				printf("you do not belong to this group");
			}
			
			printf("done\n");
			
		}
		if(pfds[1].revents&POLLIN){
			for(int i=0;i<strlen(x);i++){
				struct message msg;
				msgrcv(msqid1,&msg,sizeof(msg),x[i]-'0',0);
				if(msg.buffer[0]!='\0'){
					printf("%s\n",msg.buffer);	
				}
			}	
		}
	}
}