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
    // for reading from server creating a fifo using the pid name
    string pid = to_string(getpid());
    const char * pid_pipe = pid.c_str();  
    mkfifo(pid_pipe , 0666);
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
    int wfd = open("ChatServer" , O_WRONLY);
    // first time sending the pid to server
    write(wfd , pid.c_str() , strlen(pid.c_str()));   

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
    cout<<"process with "<<to_string(getpid())<<" has joined the chat room..... "<<endl;
    
    pthread_t readthread , writethread;
    pthread_create(&readthread , NULL , readd , NULL);
    pthread_create(&writethread , NULL , write , NULL);
    pthread_join(readthread , NULL);
    pthread_join(writethread , NULL);
    return 0;
}