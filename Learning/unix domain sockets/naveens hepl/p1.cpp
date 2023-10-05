#include <bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <csignal>
using namespace std;
#define PORT 8080
int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT);
    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 3);
    int k = sizeof(addr);
    int nsfd = accept(sfd, (struct sockaddr *)&addr, (socklen_t *)&k);
    cout << "nsfd is " << nsfd << endl;


    int usfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un uAddr;
    uAddr.sun_family = AF_UNIX;
    strcpy(uAddr.sun_path, "process_a");

    struct iovec e = {NULL, 0};
    char cmsg[CMSG_SPACE(sizeof(int))]; // allocating space for control data
    struct msghdr m = {(void *)&uAddr, sizeof(uAddr), &e, 1, cmsg, sizeof(cmsg), 0};
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(c) = nsfd;
    if (sendmsg(usfd, &m, 0) < 0)
    {
        perror("sendmsg err");
        exit(1);
    }
    close(nsfd);
    cout << "sent sfd" << endl;

    char buffer[1024];

    sockaddr_un sender_addr;
    sender_addr.sun_family = AF_UNIX;
    socklen_t sender_addr_len = sizeof(sender_addr);
    // strcpy(sender_addr.sun_path, "process_a");

    cout << "h1" << endl;
    ssize_t bytes_received = recvfrom(usfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
    cout << bytes_received << endl;

    if (bytes_received == -1)
    {
        perror("recvfrom");
        close(usfd);
        return 1;
    }

    cout << buffer << endl;

    return 0;
}
