#include<bits/stdc++.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
using namespace std;

// opening the shared memory
key_t key=ftok("c2.txt",65);
int shmid=shmget(key,1024,0666|IPC_CREAT);
key_t key1=ftok("c1.txt",65);
int shmid1=shmget(key1,1024,0666|IPC_CREAT);

void handler(int sig){
    cout<<"P2 doing the operation ->"<<endl;
    int * fromC1=(int*)shmat(shmid1,(void*)0,0);
    cout<<"P2 read "<<*fromC1<<" from C1"<<endl;
    int * toC2=(int*)shmat(shmid,(void*)0,0);
    *toC2=*fromC1+1;
    cout<<"P2 wrote "<<*toC2<<" to C2"<<endl<<endl;
}

int main(){
    signal(SIGUSR1,handler);
    cout<<"this is from p2 pid = "<<getpid()<<endl;

    sleep(2);
    // writing own pid into the shm
    int *str=(int*)shmat(shmid,(void*)0,0);
    *str=getpid();

    // getting P1s pid from shm
    int* str2=(int*)shmat(shmid1,(void*)0,0);
    int P1pid=*str2;

    *str2=1;
    sleep(3);

    // got them pids lmaoo
    while(1){
        sleep(1);
        kill(P1pid,SIGUSR1);
        pause();
    }
}