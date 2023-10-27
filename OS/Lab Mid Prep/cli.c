#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

int c;
void hfunc(int signo){
	if(c!=0){
		kill(c,SIGKILL);
		printf("\n");
	}
}

int main(int argc,char* argv[]){
	char s[100];
	signal(SIGINT,hfunc);
	printf("bt3f03@selab70 %d:~$ ",getpid());
	fgets(s,100,stdin);
	while(strcmp(s,"exit\n")){
		char cmdargu[20][100];
		int size=0;
		strcpy(cmdargu[size],"");
		for(int i=0;i<strlen(s);i++){
			if(s[i]==' '){
				size++;
				strcpy(cmdargu[size],"");
			}
			else{
				char x[2];
				x[0]=s[i];
				x[1]='\0';
				strcat(cmdargu[size],x);
			}
		}
		cmdargu[size][strlen(cmdargu[size])-1]='\0';
		size++;
		int flag=1;
		if(strcmp(cmdargu[size-1],"&")==0){
			flag=0;
		}
		else{
			flag=2;
		}
		c=0;
		c=fork();
		if(c==0){
			signal(SIGINT,SIG_DFL);
			//system(s);
			char* arr[size+1];
			for(int j=0;j<size;j++){
				arr[j]=(char *) malloc (strlen(cmdargu[j]));
				strcpy(arr[j],cmdargu[j]);
			}
			arr[size]=NULL;
			execvp(arr[0],arr);
			return 0;
		}
		else{
			if(flag==2){
				wait(NULL);
			}
			c=0;
			char x[100];
			printf("bt3f03@selab70 %d:~$ ",getpid());
			fgets(x,100,stdin);
			strcpy(s,x);
		}
		
	}
	printf("exited\n");
	return 0; 
}
