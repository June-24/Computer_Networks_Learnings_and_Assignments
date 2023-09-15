#include<bits/stdc++.h>
#include<unistd.h>
// wait
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(sfd==-1){
        perror("socket error");
        return -1;
    }
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family=AF_INET;
    // if overo ther laptop, just chnage the ip address to the receivers ip
    ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.0");
    // ServerAddr.sin_addr.s_addr=inet_addr("2401:4900:4e2a:1f16:8224:151:83a3:2c48");

    ServerAddr.sin_port=htons(8888);

    if(connect(sfd,(struct sockaddr*)&ServerAddr,sizeof(ServerAddr))==-1){
        perror("connect error");
        return -1;
    }
    label:
    cout<<"connected to server"<<endl;
    string t;
    cout<<"enter the string"<<endl;
    getline(cin,t);
    // cin>>t;
    ssize_t bytesSent=send(sfd,t.c_str(),t.length(),0);
    if(bytesSent==-1){
        perror("send error");
        return -1;
    }
    goto label;
    

}