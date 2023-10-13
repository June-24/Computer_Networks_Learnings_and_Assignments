#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
using namespace std;
struct sockaddr_in addr, cAddr;
int adrlen = sizeof(addr);

void createSocket(int &sfd)
{
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("tcp socket err");
        exit(1);
    }
}

void setSockOpt(int &sfd)
{
    int opt = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    {
        perror("sockopt err");
        exit(1);
    }
}

void bindSocket(int &sfd, int port)
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(sfd, (struct sockaddr *)&addr, adrlen) < 0)
    {
        perror("bind err");
        exit(1);
    }
}

void listenSocket(int &sfd)
{
    if (listen(sfd, 3) < 0)
    {
        perror("listen err");
        exit(1);
    }
}
string name;
int main()
{
    cout << "Enter the name: ";
    cin >> name;
    int sfd;
    createSocket(sfd);
    setSockOpt(sfd);
    int port;
    cout << "Enter the port to connect to: ";
    cin >> port;
    bindSocket(sfd, port);
    listenSocket(sfd);
    cout << "Waiting to accept" << endl;
    struct pollfd pfd[10];
    pfd[0].fd = sfd;
    pfd[0].events = POLLIN;
    int count = 1;
    while (1)
    {
        int r = poll(pfd, count, 10);
        if (r > 0)
        {
            for (int i = 0; i < count; i++)
            {
                if (i == 0 && pfd[0].revents & POLLIN)
                {

                    int nsfd = accept(sfd, (struct sockaddr *)&cAddr, (socklen_t *)&adrlen);
                    if (nsfd < 0)
                        cout << "connection error";
                    cout << "new client connected " << endl;
                    pfd[count].fd = nsfd;
                    pfd[count].events = POLLIN;
                    count++;
                }
            }
        }
        for (int i = 1; i < count; i++)
        {
            string send_msg = "from " + name;
            send(pfd[i].fd, send_msg.c_str(), sizeof(send_msg), 0);
            sleep(1);
        }
    }
}