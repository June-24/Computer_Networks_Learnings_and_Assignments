#include<bits/stdc++.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
using namespace std;

// opening the shared memory
key_t key=ftok("c1.txt",65);
int shmid=shmget(key,1024,0666|IPC_CREAT);
key_t key1=ftok("c2.txt",65);
int shmid1=shmget(key1,1024,0666|IPC_CREAT);

void handler(int sig){
    cout<<"P1 doing the operation -> "<<endl;
    int * fromC2=(int*)shmat(shmid1,(void*)0,0);
    cout<<"P1 read "<<*fromC2<<" from C2"<<endl;
    int * toC1=(int*)shmat(shmid,(void*)0,0);
    *toC1=*fromC2+1;
    cout<<"P1 wrote "<<*toC1<<" to C1"<<endl<<endl;
}
int main(){
    signal(SIGUSR1,handler);
    cout<<"this is from p1 pid = "<<getpid()<<endl;
    
    // writing own pid into the shm
    int *str=(int*)shmat(shmid,(void*)0,0);
    *str=getpid();
    sleep(4);

    // reading other pid from shared memory
    int *str2=(int*)shmat(shmid1,(void*)0,0);
    int P2pid=*str2;
    
    *str2=0;
    sleep(3);

    // got them pids lmaooo
    while(1){
        sleep(1);
        kill(P2pid,SIGUSR1);
        pause();
    }
}