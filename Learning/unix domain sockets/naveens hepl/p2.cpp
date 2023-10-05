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
using namespace std;
#define PORT 8080
int main()
{
    int usfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    unlink("process_a");
    strcpy(un.sun_path, "process_a");

    // if (bind(usfd, (struct sockaddr *)&un, sizeof(un)) < 0)
    // {
    //     perror("usfd bind err");
    //     return 1;
    // }
    char buf[512];
    struct iovec e = {buf, 512};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr m = {NULL, 0, &e, 1, cmsg, sizeof(cmsg), 0};
    if (recvmsg(usfd, &m, 0) < 0)
    {
        perror("usfd recvmsg err");
        exit(1);
    }
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    int nsfd = *(int *)CMSG_DATA(c); // receive file descriptor
    cout << nsfd << endl;

    char buf1[100];
    recv(nsfd, buf1, sizeof(buf), 0);
    cout << buf1 << endl;

    sockaddr_un dest_addr{};
    dest_addr.sun_family = AF_UNIX;
    strcpy(dest_addr.sun_path, "process_a");

    ssize_t bytes_sent = sendto(usfd, buf1, strlen(buf1), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    cout << bytes_sent << endl;
    if (bytes_sent == -1)
    {
        perror("sendto");
        close(usfd);
        return 1;
    }

    return 0;
}
