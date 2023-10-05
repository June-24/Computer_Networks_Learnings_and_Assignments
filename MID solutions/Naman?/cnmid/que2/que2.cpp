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

vector<int> socketfds;
void func (int signo)
{
    int c = 0;
    c = fork();

    if (c==0)
    {
        sleep(1);
        int ifd = socketfds[0];
        dup2(ifd , 1);
        sleep(0.5);
        execv("./E" , NULL);
    }
    else
    {
        close(socketfds[0]);
        sleep(1);
        cout<<socketfds.size()<<endl;
        cout<<"Bye bye nsfd 0"<<endl;
        socketfds.erase(socketfds.begin());
        cout<<socketfds.size()<<endl;
    }
}

int main ()
{
    cout<<"my pid is "<<getpid()<<endl;
    signal (SIGUSR1 , func);
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3001);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind (sfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    }
  
    int listenerror = listen(sfd , 10);
    if (listenerror == -1)
    {
        cout<<"Listening error is found "<<endl;
        return 0;
    }

    int fd1 = 10;
    // mkfifo("dont" , 0666);
    // int fd1 = open("dont" , O_RDONLY);

    struct pollfd pfd[3];
    pfd[0].fd = sfd; pfd[0].events = POLLIN;
    pfd[1].fd = fd1; pfd[1].events = POLLIN;
    pfd[2].fd = 0; pfd[2].events = POLLIN;

    while (1)
    {
        int s = poll (pfd , 3 , 1000);
        if (s<=0)
            continue;
        
        for (int i=0;i<3;i++)
        {
            if (i==0 && (pfd[0].revents & POLLIN))
            {
                struct sockaddr * clientaddr;
                socklen_t len = sizeof(clientaddr);

                int newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);
                if (newfd==-1)
                {
                    cout<<"not"<<endl;
                    continue;
                }
                cout<<"New client"<<endl; 
                socketfds.push_back(newfd);
            }
            else if (pfd[i].revents & POLLIN)
            {
                char buffer[100];
                int n = read(pfd[i].fd , buffer , sizeof(buffer)-1);
                buffer[n] = '\0';
                cout<<"read "<<buffer<<endl;

                for (auto j : socketfds)
                {
                    send(j , buffer , strlen(buffer)+1 , 0);
                }
            }
        }
    }
}