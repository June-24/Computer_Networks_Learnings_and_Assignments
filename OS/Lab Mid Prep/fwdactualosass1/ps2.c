#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<time.h>


char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
	
}


void ps(){
	DIR* d;
	char y[20];
	strcpy(y,ttyname(STDIN_FILENO));
	printf("%s\n",y);
	struct dirent* cont;
	d=opendir("/proc"); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			if(cont->d_type == DT_DIR){
				int pid=atoi(cont->d_name);
				if(pid!=0){
					char path[256];
                			snprintf(path, sizeof(path), "/proc/%d/stat", pid);
                			FILE* statfile=fopen(path,"r");
                			if(statfile){
                				int ppid;
                				char name[256], state;
					    	fscanf(statfile, "%d %s %c", &ppid, name, &state);
					    	printf("%d\t%s\t%c\n", ppid, name, state);
                			}
                			fclose(statfile);
				}
			}
		}
		else{
			break;
		}
	}
}

void normalps(){
	DIR* d;
	char y[20];
	strcpy(y,ttyname(STDIN_FILENO));
	printf("%s\n",y);
	struct dirent* cont;
	d=opendir("/proc"); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			if(cont->d_type == DT_DIR){
				int pid=atoi(cont->d_name);
				if(pid!=0){
					char path[256];
                			snprintf(path, sizeof(path), "/proc/%d/stat", pid);
                			FILE* statfile=fopen(path,"r");
                			if(statfile){
                				int ppid;
                				char name[256], state, link[256], path2[256];
					    	fscanf(statfile, "%d %s %c", &ppid, name, &state);
					    	snprintf(path2, sizeof(path2), "/proc/%d/fd/0", pid);
					    	int len=readlink(path2,link,sizeof(link)-1);
					    	link[len]='\0';
					    	if(strcmp(link,y)==0){
					    		printf("%d\t%s\t%s\t%c\n", ppid, link, name, state);
					    	}
                			}
                			fclose(statfile);
				}
			}
		}
		else{
			break;
		}
	}

}

void userps(){
	DIR* d;
	char y[20];
	strcpy(y,ttyname(STDIN_FILENO));
	printf("%s\n",y);
	struct dirent* cont;
	d=opendir("/proc"); 
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			if(cont->d_type == DT_DIR){
				int pid=atoi(cont->d_name);
				if(pid!=0){
					char path[256];
                			snprintf(path, sizeof(path), "/proc/%d/stat", pid);
                			FILE* statfile=fopen(path,"r");
                			if(statfile){
                				int ppid;
                				char name[256], state, link[256], path2[256];
                				int arr[15];
                				long int time[3];
					    	fscanf(statfile, "%d %s %c %d %d %d %d %d %d %d %d %d %d %d %ld %ld %ld", &ppid, name, &state,&arr[0],&arr[1],&arr[2],&arr[3],&arr[4],&arr[5],&arr[6],&arr[7],&arr[8],&arr[9],&arr[10],&time[0],&time[1],&time[2]);
					    	snprintf(path2, sizeof(path2), "/proc/%d/fd/0", pid);
					    	int timee = (int)((double)(time[0] + time[1]) / sysconf(_SC_CLK_TCK));
					    	char time_s[256];
 						sprintf(time_s, "%02lu:%02lu:%02lu",(timee / 3600) % 3600, (timee / 60) % 60, timee % 60);
					    	int len=readlink(path2,link,sizeof(link)-1);
					    	link[len]='\0';
					    	if(len>0&&link[5]!='n'&&link[0]=='/'){
					    		printf("%d\t%s\t%s\t%c\t%s\n", ppid, link, name, state,time_s);
					    	}
                			}
                			fclose(statfile);
				}
			}
		}
		else{
			break;
		}
	}
}

void psa(){
	DIR* d;
	char y[20];
	strcpy(y,ttyname(STDIN_FILENO));
	printf("%s\n",y);
	struct dirent* cont;
	d=opendir("/proc");
	while(1)
	{
		if((cont=readdir(d))!=NULL){
			if(cont->d_type == DT_DIR){
				int pid=atoi(cont->d_name);
				if(pid!=0){
					char path[256];
                			snprintf(path, sizeof(path), "/proc/%d/stat", pid);
                			FILE* statfile=fopen(path,"r");
                			if(statfile){
                				int ppid;
                				char name[256], state, link[256], path2[256];
					    	fscanf(statfile, "%d %s %c", &ppid, name, &state);
					    	snprintf(path2, sizeof(path2), "/proc/%d/fd/0", pid);
					    	int len=readlink(path2,link,sizeof(link)-1);
					    	link[len]='\0';
					    	if(len>0&&link[5]!='n'&&link[0]=='/'){
					    		printf("%d\t%s\t%s\t%c\n", ppid, link, name, state);
					    	}
                			}
                			fclose(statfile);
				}
			}
		}
		else{
			break;
		}
	}
}
int main(int argc,char* argv[]){
	if(argc==2){
		if(strcmp(argv[1],"-ae")==0){
			ps();
		}
		else if(strcmp(argv[1],"-u")==0){
			userps();
		}
		else if(strcmp(argv[1],"-a")==0){
			psa();
		}
	}
	else{
		normalps();
	}
	return 0; 
}
