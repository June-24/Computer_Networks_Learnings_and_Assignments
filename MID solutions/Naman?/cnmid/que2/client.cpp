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
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3001);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");

    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }
    cout<<"connected"<<endl;
    struct pollfd pfd[1];
    pfd[0].fd = 0; pfd[0].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , 1 , 1000);
       
        if (s>0)
        if (pfd[0].revents & POLLIN)
        {
            string str;
            getline(cin , str);
            const char * buf = str.c_str();
            cout<<"sending the text : "<<str<<endl;
            send(sfd , buf , strlen(buf) , 0);
        }

        char buffer[200];
        int n = recv(sfd , buffer , sizeof(buffer) , MSG_DONTWAIT);
        if (n>0)
            cout<<"got the text "<<buffer<<endl;
    }
}