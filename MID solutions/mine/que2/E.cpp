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
    int ofd = 10;
    dup2(1 , ofd);

    int tty_fd=open("/dev/tty",O_WRONLY);
    dup2(tty_fd,1);

    cout<<"E started now: : :  "<<endl;

    while (1)
    {
        char buffer[100];
        int n=recv(ofd , buffer , sizeof(buffer) , 0);
        if(n>0){
            buffer[n]='\0';
            cout<<"E process received: "<<buffer<<endl;
            fflush(stdout);

        }
    }
}