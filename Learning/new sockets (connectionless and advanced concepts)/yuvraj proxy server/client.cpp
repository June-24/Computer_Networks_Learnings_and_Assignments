#include <bits/stdc++.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
#define PORT 3030
int main()
{
    int psfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(psfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        close(psfd);
        return 1;
    }
    cout << "connected to the proxy server" << endl;
    char buffer[1024];
    char service[1024];
    cout << "enter the service number" << endl;
    fgets(service, sizeof(service), stdin);
    send(psfd, service, sizeof(service), 0);
    cout << "enter the message to special server-" << service << endl;
    fgets(buffer, sizeof(buffer), stdin);
    send(psfd, buffer, sizeof(buffer), 0);
    cout << buffer << "- is sent to special server-" << service << endl;
    close(psfd);

    return 0;
}
