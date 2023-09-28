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
    char buf[100];
    recv(1 , buf , sizeof(buf) , 0);
    cout<<buf<<endl;

    struct pollfd pfd[5]; dup2(0 , pfd[0].fd); pfd[0].events = POLLIN;

    int tty_fd=open("/dev/tty",O_WRONLY);
    dup2(tty_fd,1);
    cout<<"came here boi "<<endl;
    
    int reuse = 1;
    if (setsockopt(pfd[0].fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    if (setsockopt(pfd[0].fd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEPORT) failed");

    int cnt = 1;

    while (1)
    {
        int s = poll (pfd , cnt , 1000);

        if (s<=0)
            continue;
        
        cout<<s<<endl;

        for (int i=0;i<cnt;i++)
        {
            if (i==0 && (pfd[0].revents & POLLIN))
            {
                struct sockaddr_in clientaddress;
                socklen_t len = sizeof(clientaddress);
                cout<<"naman"<<endl;
                int newsfd = accept (pfd[0].fd , (sockaddr *)&clientaddress , &len);
                if (newsfd==-1)
                {
                    cout<<"connection faliure"<<endl;
                    return 0;
                }
                cout<<"Connnection done "<<endl;

                pfd[cnt].fd = newsfd; pfd[cnt].events = POLLIN;
                cnt++;
            }
            else
            {
                if (pfd[i].revents & POLLIN)
                {
                    char buffer[100];
                    recv(pfd[i].fd , buffer , sizeof(buffer)+1 , 0);
                    cout<<buffer<<endl;
                }
            }
        }
        
    }
}