#include <bits/stdc++.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
#include<pthread.h>
using namespace std;
struct my_buffer 
{
    long type;
    char data[512];
};
// creating the msg queues
int key_server = ftok("./server.txt" , 65);
int key_client = ftok("./client.txt" , 65);
int msqid_server = msgget(key_server , IPC_CREAT | 0666);
int msqid_client = msgget(key_client , IPC_CREAT | 0666);

void* readd(void* arg){
    while(1){
        struct my_buffer temp1;
        int t = msgrcv(msqid_client , &temp1 , sizeof(temp1.data) , getpid() , MSG_NOERROR | IPC_NOWAIT);
        if (t!=-1){
            temp1.data[t] = '\0';
            cout<<"User #"<<temp1.type<<" : "<<temp1.data<<endl;
        }
    }
}

void* writee(void* args){
    while(1){
        struct my_buffer temp;
        fgets(temp.data , 100 , stdin);
        temp.type = getpid();
        msgsnd(msqid_server , &temp , strlen(temp.data) , 0);
    }
}
int main ()
{
    // sending teh pid first to server
    long pid = (long)getpid();
    struct my_buffer sending_pid;
    sending_pid.type = 1;//first time is 1
    char const* pidd = (to_string(getpid())).c_str();
    strncpy(sending_pid.data,pidd,strlen(pidd));
    sending_pid.data[strlen(pidd)] = '\0';
    msgsnd(msqid_server , &sending_pid , strlen(sending_pid.data) , 0);// sending the pid to server

    pthread_t read_thread , write_thread;
    pthread_create(&read_thread , NULL , readd , NULL);
    pthread_create(&write_thread , NULL , writee , NULL);
    pthread_join(read_thread , NULL);
    pthread_join(write_thread , NULL);
    return 0;
}
