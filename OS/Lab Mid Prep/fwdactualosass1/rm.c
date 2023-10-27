#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>

int main(int argc,char* argv[]){
	for(int i=1;i<argc;i++){
		if(strcmp(argv[1],"-i")==0){
			printf("remove file %s?(n/y)",argv[2]);
			char c;
			scanf("%c",c);
			if(c=='n')
			i=i+=2;
			else
			i++;
		}
		int c=remove(argv[i]);
		if(c==0)
		printf("successfully deleted\n");
		else
		printf("failed to delete\n");
	}
	return 0; 
}
