#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

// struct pollfd 
// {
//     int fd;
//     short events;
//     short revents;
// };

int main ()
{
    string pidd = to_string(getpid());

    cout<<"Joining process "<<pidd<<" in the chat room ...."<<endl;
    cout<<endl;

    char makepipe[pidd.length()+1];
    for (int i=0;i<pidd.length();i++)
        makepipe[i] = pidd[i];
    makepipe[pidd.length()] = '\0';

    mkfifo(makepipe , 0666);

    const char * pid = pidd.c_str();

    int wfd = open("chattalks" , O_WRONLY);

    write(wfd , pid , strlen(pid));   

    int rfd = open(pid , O_RDONLY);

    struct pollfd pfd[2];
    int users = 0;

    pfd[0].fd = 0; pfd[0].events = POLLIN; 
    pfd[1].fd = rfd; pfd[1].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , 2 , 1000);
        if (s>0)
        {
            char buffer[1024];
            if (pfd[0].revents & POLLIN)
            {   
                cin>>buffer;
                cout<<"sending the answer : "<<buffer<<endl;
                string newtext = pidd + " " + buffer;
                const char * newtextt = newtext.c_str();
                write(wfd , newtextt , strlen(newtextt));
            }
            if (pfd[1].revents & POLLIN)
            {
                cout<<"reading one called"<<endl;
                char readbuffer[1024];
                read(rfd , readbuffer , sizeof(readbuffer));
                cout<<readbuffer<<endl;
            }
        }
    }
}