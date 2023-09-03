#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
#include<pthread.h>
using namespace std;
void* readd(void* arg){
    // for reading from server
    string pid = to_string(getpid());
    char makepipe[pid.length()+1];
    for (int i=0;i<pid.length();i++)
        makepipe[i] = pid[i];
    makepipe[pid.length()] = '\0';
    // making that fifo
    mkfifo(makepipe , 0666);
    const char * pid_pipe = pid.c_str();  
    int rfd = open(pid_pipe , O_RDONLY);
    while(1){
        char readbuffer[1024];
        read(rfd , readbuffer , sizeof(readbuffer));
        cout<<readbuffer<<endl;
    }
}
void* write(void* arg){
    // for writing to server
    string pid = to_string(getpid());
    int wfd = open("NITWGDB" , O_WRONLY);
    while(1){
        string strl;
        getline(cin,strl);
        cout<<"sending the message : "<<strl<<endl;
        string newtext = pid + ":" + strl;
        const char * newtextt = newtext.c_str();
        write(wfd , newtextt , strlen(newtextt));
    }
}
int main ()
{
    // getting pid
    string pid = to_string(getpid());
    cout<<"process with "<<pid<<" has joined the chat room..... "<<endl;

    // to make new fifo, its name should be the pid_pipe number
    char makepipe[pid.length()+1];
    for (int i=0;i<pid.length();i++)
        makepipe[i] = pid[i];
    makepipe[pid.length()] = '\0';
    // making that fifo
    mkfifo(makepipe , 0666);

    const char * pid_pipe = pid.c_str();
    int wfd = open("NITWGDB" , O_WRONLY);
    // first time just sending the pid_pipe number to put in map
    write(wfd , pid_pipe , strlen(pid_pipe));   
    int rfd = open(pid_pipe , O_RDONLY);
    
    pthread_t readthread , writethread;
    pthread_create(&readthread , NULL , readd , NULL);
    pthread_create(&writethread , NULL , write , NULL);
    pthread_join(readthread , NULL);
    pthread_join(writethread , NULL);
    return 0;
}