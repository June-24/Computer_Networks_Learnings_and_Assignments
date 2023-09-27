#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/sendfile.h>
using namespace std;


int main(){
    // server using connection less socket
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd == -1){
        cout<<"Error in creating socket"<<endl;
        exit(1);
    }
    cout<<"Socket created successfully"<<endl;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sfd, (struct sockaddr *)&server, sizeof(server));
    
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buffer[1024];
    while(1){
        int n = recvfrom(sfd, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        getpeername(sfd, (struct sockaddr *)&client, &len);
        
        if(n>0){
        buffer[n] = '\0';
            cout<<"client port: "<<ntohs(client.sin_port)<<endl; // ntohs is used to convert network byte order to host byte order
            cout<<"more data: "<<inet_ntoa(client.sin_addr)<<endl; // inet_ntoa is used to convert network byte order to host byte order
            cout<<"Client: "<<buffer<<endl;
        }
    }

}