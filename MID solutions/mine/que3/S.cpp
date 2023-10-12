#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

int sfd = 10;
void * talks (void * args)
{
    struct sockaddr * clientaddr;
    socklen_t len = sizeof(clientaddr);

    int newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);
    if (newfd==-1)
    {
        cout<<"not"<<endl;
        return NULL;
    }
    cout<<"I am S process and accepted connection  "<<endl<<endl<<endl;

    while (1)
    {
        char buffer[100];
        int n=recv(newfd , buffer , sizeof(buffer) , 0);
        if(n>0)
        {
            buffer[n]='\0';
            cout<<"I am S process and received: "<<buffer<<endl<<endl<<endl;
            
        }
    }
}

void func (int signo)
{
    cout<<"got signal "<<endl<<endl<<endl;
    pthread_t stuff;
    pthread_create(&stuff , NULL , talks , NULL);

    pthread_join(stuff , NULL);
}

int main ()
{
    dup2(1 , sfd);

    int tty_fd=open("/dev/tty",O_WRONLY);
    dup2(tty_fd,1);

    int reuse = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
    perror("setsockopt(SO_REUSEPORT) failed");

    cout<<"I am Service 1 and my pid is "<<getpid()<<endl<<endl<<endl;

    signal (SIGUSR1 , func);
    while (1)
    {

    }
}