#include<iostream>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
using namespace std;

int main(){
    key_t t=ftok("shmfile",65);
    int shmid=shmget(t,1024,0666|IPC_CREAT);
    char * str=(char*)shmat(shmid,(void*)0,0);

    printf("Write Data : ");
    fgets(str, 100, stdin);
    printf("Data written in memory: %s\n",str);
    shmdt(str);
}