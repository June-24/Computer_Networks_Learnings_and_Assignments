#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main()
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    // if overo ther laptop, just chnage the ip address to the servers ip
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    // "SERVER_EXTERNAL_IP_ADDRESS"
    // ServerAddr.sin_addr.s_addr = inet_addr("2401:4900:4e2a:1f16:8224:151:83a3:2c48");
    // ServerAddr.sin_addr.s_addr = inet_addr("SERVER_EXTERNAL_IP_ADDRESS");


    ServerAddr.sin_port = htons(8888);

    int ret = bind(sfd, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
    if (ret == -1)
    {
        perror("binding error");
        return -1;
    }

    if (listen(sfd, 10) == -1)
    {
        perror("listen error");
        return -1;
    }

    cout << "Server open at port 8888" << endl;

    while (true)
    {
        struct sockaddr_in ClientAddr;
        socklen_t len = sizeof(ClientAddr);
        int nsfd = accept(sfd, (struct sockaddr *)&ClientAddr, &len);
        if (nsfd == -1)
        {
            perror("accept error");
            continue; // Continue listening for the next client
        }

        int c = fork();
        if (c > 0)
        {
            // Parent process
            close(nsfd);
        }
        else
        {
            // Child process
            close(sfd);
            char buffer[1024];
            ssize_t bytesRead;

            while ((bytesRead = recv(nsfd, buffer, sizeof(buffer) - 1, 0)) > 0)
            {
                buffer[bytesRead] = '\0';
                cout << "Client: " << buffer << endl;
            }

            if (bytesRead == -1)
            {
                perror("recv error");
            }

            close(nsfd);
            return 0;
        }
    }

    close(sfd);
    return 0;
}
