#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    // opening the socket
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket error");
        return -1;
    }
    // initialising the server address
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family=AF_INET;
    ServerAddr.sin_addr.s_addr=INADDR_ANY;
    ServerAddr.sin_port=htons(6969);
    // binding the socket to the server address
    if(bind(sfd,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr))==-1){
        perror("binding error");
        return -1;
    }
    // listening on the socket
    if(listen(sfd,10)==-1){
        perror("listening error");
        return -1;
    }
    cout<<"Server is ready and listening on port 6969"<<endl;
    // accepting the connection
    struct sockaddr_in ClientAddr;
    socklen_t ClientAddrLen=sizeof(ClientAddr);
    int nsfd=accept(sfd,(struct sockaddr*)&ClientAddr,&ClientAddrLen);
    close(sfd);
    if(nsfd==-1){
        perror("accepting error");
        return -1;
    }
    // reading from the socket
    char buff[100];
    ssize_t bytesRead=recv(nsfd,buff,100,0);
    if(bytesRead==-1){
        perror("reading error");
        return -1;
    }
    buff[bytesRead]='\0';
    cout<<"Received from client : "<<buff<<endl;
    // writing to the socket
    string t;
    cout<<"Enter the message to be sent to the client : ";
    getline(cin,t);
    ssize_t bytesWritten=send(nsfd,t.c_str(),t.length(),0);
    if(bytesWritten==-1){
        perror("writing error");
        return -1;
    }

    close(nsfd);
    close(sfd);


}
