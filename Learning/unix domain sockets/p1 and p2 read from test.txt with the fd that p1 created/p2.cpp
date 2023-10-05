#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <csignal>
#include<fcntl.h>
using namespace std;
int main(){
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    struct sockaddr_un serveraddr;
    serveraddr.sun_family=AF_UNIX;
    strcpy(serveraddr.sun_path,"./socketfile");
    unlink("./socketfile");
    if(bind(usfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
        perror("bind");
        exit(1);
    }
    cout<<"Waiting for file descriptor from P1\n";
    char buff[512];
    struct iovec E={buff,sizeof(buff)};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr M={(void*)&serveraddr,sizeof(serveraddr),&E,1,cmsg,sizeof(cmsg),0};
    if(recvmsg(usfd,&M,0)<0){
        perror("recvmsg");
        exit(1);
    }
    struct cmsghdr* c=CMSG_FIRSTHDR(&M);
    int fd=*(int*)CMSG_DATA(c);
    cout<<"File descriptor received from P1\n";
    cout<<"Contents of file:\n";
    char buffer;
    int newlineCount = 0;

    while (read(fd, &buffer, 1) > 0)
    {
        cout << buffer;
        if (buffer == '\n')
            if (++newlineCount == 2)
                break; //exit the loop after reading two newline characters
    }
}