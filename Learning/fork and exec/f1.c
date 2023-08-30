#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>

int main(){
    int c=0;
    c=fork();
    if(c>0){
        // parent process
        // wait(NULL);
        printf("this is parent process\n\n");
    }
    else{
        // child process
        printf("this is the child part\n\n");
    }
}