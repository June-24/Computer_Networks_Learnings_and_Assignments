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
    cout<<"P2 received signal from P1"<<endl;
}

int main(){
    signal(SIGUSR1,handler);
    cout<<"this is from p2 pid = "<<getpid()<<endl;
    // writing the pid into the shared memory
    sleep(2);
    
    char *str=(char*)shmat(shmid,(void*)0,0);
    const char* str1=to_string(getpid()).c_str();
    strcpy(str,str1);
    
    
    // reading other pid from shared memory

    char *str2=(char*)shmat(shmid1,(void*)0,0);
    string P1pid=string(str2);
    // got them pids lmaoo
    while(1){
        sleep(1);
        kill(stoi(P1pid),SIGUSR1);
        pause();
    }
}