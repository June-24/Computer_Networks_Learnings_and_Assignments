#include <bits/stdc++.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <csignal>
#include <fcntl.h>
using namespace std;
int main()
{
    // fd for reading from file
    int fd = open("test.txt", O_RDONLY);
    // reading first two lines
    char buffer;
    int newlineCount = 0;

    while (read(fd, &buffer, 1) > 0)
    {
        cout << buffer;
        if (buffer == '\n')
            if (++newlineCount == 2)
                break; //exit the loop after reading two newline characters
    }
    // unix domain socket for sending the fd to P2
    int usfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un serveraddr;
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, "./socketfile");

    // initialising the msghdr structure
    struct iovec E = {NULL, 0};
    char cmsg[CMSG_SPACE(sizeof(int))]; // allocating space for control data
    struct msghdr M = {(void *)&serveraddr, sizeof(serveraddr), &E, 1, cmsg, sizeof(cmsg), 0};
    struct cmsghdr *c = CMSG_FIRSTHDR(&M);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(c) = fd;

    // sending the fd to P2
    if (sendmsg(usfd, &M, 0) < 0)
    {
        perror("sendmsg");
        exit(1);
    }
    cout << "File descriptor sent to P2\n";
}