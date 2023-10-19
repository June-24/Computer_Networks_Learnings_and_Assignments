#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;
struct sockaddr_in addr, cAddr;
int adrlen = sizeof(addr);

int main()
{
    int sfd;
        if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("tcp socket err");
        exit(1);
    }
        int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("sockopt err");
        exit(1);
    }
        addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    if (bind(sfd, (struct sockaddr *)&addr, adrlen) < 0)
    {
        perror("bind err");
        exit(1);
    }
        if (listen(sfd, 3) < 0)
    {
        perror("listen err");
        exit(1);
    }
    cout << "Waiting to accept" << endl;
    int nsfd = accept(sfd, (struct sockaddr *)&cAddr, (socklen_t *)&adrlen);
    close(sfd);
    if (nsfd < 0)
    {
        perror("accept err");
        exit(0);
    }
    cout << "Accepted" << endl;
    char buff[11] = "Server msg";
    while (1)
    {
        send(nsfd, buff, sizeof(buff), 0);
        char msg[50] = {'\0'};
        int n = recv(nsfd, msg, sizeof(msg), 0);
        if (n > 0)
        {
            cout << msg << endl;
            sleep(1);
        }
    }
}