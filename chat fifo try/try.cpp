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


// it shows that poll is working ... 

int main ()
{

    // int ifd1 = open("./input1.txt" , O_RDONLY);
    // int ifd2 = open("./input2.txt" , O_RDONLY);
    int ifd1 = 1;
    int ifd2 = 2;
    struct pollfd pfd[3];
    pfd[0].fd = ifd1; pfd[0].events = POLLIN;
    pfd[1].fd = ifd2; pfd[0].events = POLLIN;
    pfd[2].fd = 0; pfd[2].events = POLLIN;

    while (1)
    {   
        int s = poll(pfd , 3 , 10);

        if (s>0)
        {
            for (int i=0;i<3;i++)
            {
                if (pfd[i].revents & POLLIN)
                {   
                    char buffer[200];
                    read(pfd[i].fd , buffer , 200);
                    cout<<buffer<<endl;
                }
            }
        }
        else if (s==0)
        {
            cout<<"No events occured "<<endl;
        }
    }
}