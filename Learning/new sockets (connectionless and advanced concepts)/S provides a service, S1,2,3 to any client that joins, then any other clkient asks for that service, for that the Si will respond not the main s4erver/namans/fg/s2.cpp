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

int main ()
{
    struct pollfd pfd[10];
    dup2(1 , pfd[0].fd);
    pfd[0].events = POLLIN;

    int tty_fd=open("/dev/tty",O_WRONLY);
    dup2(tty_fd,1);

    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(3001);

    int binderror = bind (sfd , (struct sockaddr*)&serveraddress , sizeof(serveraddress));
    if (binderror==-1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    } 
    if (listen (sfd , 10)==-1)
    {
        cout<<"error in listening "<<endl;
        return 0;
    }

    while (1)
    {
        struct sockaddr_in clientaddress;
        socklen_t len = sizeof(clientaddress);

        int newsfd = accept (pfd[0].fd , (sockaddr *)&clientaddress , &len);
        if (newsfd==-1)
        {
            cout<<"connection faliure"<<endl;
            return 0;
        }
        cout<<"Connnection done "<<endl;
    }
}