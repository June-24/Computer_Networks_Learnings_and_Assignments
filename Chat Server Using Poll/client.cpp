#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

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
    
    // creating the pollfds 
    struct pollfd pfd[2];
    int users = 0;

    // initialising the pollfds
    pfd[0].fd = 0; pfd[0].events = POLLIN; 
    pfd[1].fd = rfd; pfd[1].events = POLLIN;

    while (1)
    {
        // getting the bool value
        int ret = poll(pfd , 2 , 100);
        if (ret>0)
        {
            char buffer[1024];
            // if it is ready to take input, we take it
            if (pfd[0].revents & POLLIN)
            {   
                string strl;
                getline(cin,strl);
                cout<<"sending the message : "<<strl<<endl;
                string newtext = pid + ":" + strl;
                const char * newtextt = newtext.c_str();
                write(wfd , newtextt , strlen(newtextt));
            }
            // if it is ready to print input from famous fifo, itll print
            if (pfd[1].revents & POLLIN)
            {
                char readbuffer[1024];
                read(rfd , readbuffer , sizeof(readbuffer));
                cout<<readbuffer<<endl;
            }
        }
    }
}