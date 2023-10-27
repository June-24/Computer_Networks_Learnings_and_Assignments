#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

int main(int argc,char* argv[]){
	if(argc==1){
		while(1)
		{
			char *s=(char *)malloc(100*sizeof(char));
			read(0,s,100);
			printf("%s", s);
		}
	}
	else{
		for(int i=1;i<argc;i++){
			FILE* f=fopen(argv[i],"r");
			char buff;
			while(1){
				buff=fgetc(f);
				if(feof(f)){
					break;
				}
				printf("%c",buff);
			}
			fclose(f);
		}
	}
	return 0; 
}
