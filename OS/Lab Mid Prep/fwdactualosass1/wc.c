#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(int argc,char* argv[]){
	int flag12=0;
	for(int i=1;i<argc;i++){
		if(argv[i]=="-c"){
			i++;
			flag12=1;
		}
		else if(argv[i]=="-l"){
			i++;
			flag12=2;
		}
		else if(argv[i]=="-w"){
			flag12=3;
			i++;
		}
		FILE* f=fopen(argv[i],"r");
		char buff;
		int charcount=0,wordcount=0,linecount=0;
		int flag=0,flag2=0;
		char buff2[3];
		int size=0;
		int numint=0;
		strcpy(buff2,"");
		while(1){
			buff=fgetc(f);
			if(feof(f)){
				if(flag==0){
					wordcount++;
					linecount++;
				}
				
				break;
			}
			charcount++;
			if(buff==' '){
				wordcount++;
				size=0;
			}
			else{
			
			}
			if(buff=='\t'){
				size=0;
			}
			if(buff=='\n'){
				linecount++;
				wordcount++;
				flag=1;
				size=0;
			}
			if(buff!='\n'&&buff!='\t'&&buff!=' '){
				if(size<3){
					buff2[size++]=buff;
				}
				if(size==3&&buff2[0]=='i'&&buff2[1]=='n'&&buff2[2]=='t'){
					numint++;
				}
				flag=0;
			}
		}
		if(flag12==0){
			printf("%d %d %d %s\n",linecount,wordcount,charcount,argv[i]);
		}
		if(flag12==1){
			printf("%d %s\n",charcount,argv[i]);
		}
		if(flag12==2){
			printf("%d %s\n",linecount,argv[i]);
		}
		if(flag12==3){
			printf("%d %s\n",wordcount,argv[i]);
		}
		printf("%d\n",numint);
		fclose(f);
	}
	return 0; 
}
