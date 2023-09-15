#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>

int main(){
    printf("the pid of p2 is : %d\n\n",getpid());
    printf("this is from p2 program\n\n");
}