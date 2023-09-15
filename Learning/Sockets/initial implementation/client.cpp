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
    ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    ServerAddr.sin_port=htons(6969);
    // connecting to the server
    if(connect(sfd,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr))==-1){
        perror("connection error");
        return -1;
    }
    cout<<"Connectted to server"<<endl;
    string t;
    cin>>t;
    // sending to the server
    const char* buff=t.c_str();
    if(send(sfd,buff,strlen(buff),0)==-1){
        perror("sending error");
        return -1;
    }
    close(sfd);
    
}