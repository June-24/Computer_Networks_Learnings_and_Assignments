#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<time.h>
#include<unistd.h>

void ls(char* argv[]){
	DIR* d;
	struct dirent* cont;
	d=opendir("."); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			if(cont->d_name[0]!='.')
			printf("%s\n",cont->d_name);
		}
		else{
			break;
		}
	}
}

void lsa(){
	DIR* d;
	struct dirent* cont;
	d=opendir("."); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			printf("%s\n",cont->d_name);
		}
		else{
			break;
		}
	}
}

void lsdir(char* argv[]){
	DIR* d;
	struct dirent* cont;
	d=opendir(argv[1]); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			printf("%s\n",cont->d_name);
		}
		else{
			break;
		}
	}
}

void lsl(){
	DIR* d;
	struct dirent* cont;
	d=opendir("."); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			struct stat file_stat;
			stat(cont->d_name,&file_stat);
			if(stat(cont->d_name,&file_stat)==0){
				printf("%ld %lld %o %d %d %s %s %s %s\n",file_stat.st_nlink,(long long)file_stat.st_size,file_stat.st_mode & 0777,file_stat.st_uid,file_stat.st_gid,ctime(&file_stat.st_atime),ctime(&file_stat.st_mtime),ctime(&file_stat.st_ctime),cont->d_name); // last access date, last modif date, last status change date
			}
		}
		else{
			break;
		}
	}
}

void lsr(DIR* d,char some[]){
	struct dirent* cont;
	if(d==NULL){
		return;
	}
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			if(cont->d_name[0]!='.'){
				printf("%s\n",cont->d_name);
				if(cont->d_type==DT_DIR){
					DIR* x;
					strcat(some,"/");
					strcat(some,cont->d_name);
					x=opendir(some);
					lsr(x,some);
				}
			}
		}
		else{
			break;
		}
	}
}


int main(int argc,char* argv[]){
	if(argc==1){
		ls(argv);
	}
	else if(argc==2){
		if(strcmp(argv[1],"-l")==0){
			lsl();
		}
		else if(strcmp(argv[1],"-a")==0){
			lsa();
		}
		else if(strcmp(argv[1],"-R")==0){
			DIR* d;
			char some[100];
			strcpy(some,".");
			d=opendir("."); 
			lsr(d,some);
		}
		else{
			lsdir(argv);
		}
	}
	return 0; 
}
