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
    struct pollfd pfd[2];
    
    for (int i=0;i<2;i++)
    {
        int sfd = socket(AF_INET , SOCK_STREAM , 0);
        if (sfd == -1)
        {
            cout<<"Error in creating socket "<<endl;
            return 0;
        }

        int reuse = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEPORT) failed");

        struct sockaddr_in serveraddress;
        serveraddress.sin_family = AF_INET;
        serveraddress.sin_addr.s_addr = INADDR_ANY;
        serveraddress.sin_port = htons(4000+i);

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

        pfd[i].fd = sfd; pfd[i].events = POLLIN;
    }

    while (1)
    {
        int s = poll(pfd , 2 , 1000);
        if (s<=0)
            continue;

        if (pfd[0].revents & POLLIN)
        {
            struct sockaddr_in clientaddress;
            socklen_t len = sizeof(clientaddress);

            int newsfd = accept (pfd[0].fd , (sockaddr *)&clientaddress , &len);
            if (newsfd==-1)
            {
                cout<<"connection faliure"<<endl;
                return 0;
            }
            
            int c = 0;
            c = fork();

            if (c>0)
            {
                sleep(2);
                close(pfd[0].fd);
                close(newsfd);
                pfd[0].fd = 0;
                sleep(2);
            }
            else
            {
                cout<<"from parent executed"<<endl;
                dup2(newsfd , 1);
                dup2(pfd[0].fd , 0);
                sleep(2);
                execv("./s1" , NULL);
            }
        }
        else if (pfd[1].revents & POLLIN)
        {
            struct sockaddr_in clientaddress;
            socklen_t len = sizeof(clientaddress);

            int newsfd = accept (pfd[1].fd , (sockaddr *)&clientaddress , &len);
            if (newsfd==-1)
            {
                cout<<"connection faliure"<<endl;
                return 0;
            }
            
            int c = 0;
            c = fork();

            if (c>0)
            {
                close(pfd[1].fd);
                close(newsfd);
                sleep(2);
            }
            else
            {
                sleep(2);
                dup2(newsfd , 1);
                execv("./s2" , NULL);
            }
        }
    }
}