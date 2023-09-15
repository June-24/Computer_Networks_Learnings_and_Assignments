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

int main() {
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("socket error");
        return -1;
    }
    
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.0");
    ServerAddr.sin_port = htons(8899);

    if (connect(sfd, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr)) == -1) {
        perror("connect error");
        return -1;
    }
    
    cout << "Client 1 -> connected to server" << endl;
    cout << "Enter which service you want (s1, s2, s3): ";
    string service;
    cin >> service;
    ssize_t bytesSent = send(sfd, service.c_str(), service.length(), 0);
        if (bytesSent == -1) {
            perror("send error");
            return -1;
        }
    
    string message;
    cin.ignore(); 
    while (true) {
        cout << "Enter the message for service " << service << " (or type 'exit' to quit): ";
        getline(cin, message);
        
        if (message == "exit") {
            break;
        }
        
        ssize_t bytesSent = send(sfd, message.c_str(), message.length(), 0);
        if (bytesSent == -1) {
            perror("send error");
            return -1;
        }
    }
    
    close(sfd);
    return 0;
}
