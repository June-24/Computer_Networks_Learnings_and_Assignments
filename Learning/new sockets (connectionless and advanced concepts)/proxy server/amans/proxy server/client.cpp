#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main (){
    
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1) perror("Socket error");


    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(50001);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connection
    int k = connect (sfd , (sockaddr *)&server_address , sizeof(server_address));
    if (k==-1){
        perror("Connection error");
        return 0;
    }

    cout<<"Connected"<<endl;


    string s;
    cout<<"Which special server? : ";
    getline(cin,s);

    const char * buffer = s.c_str();
    cout<<buffer<<endl;
    send(sfd,buffer,strlen(buffer)+1,0);

    while(1){
        string s;
        cout<<"Enter something nigga : ";
        getline(cin,s);

        buffer = s.c_str();
        cout<<buffer<<endl;
        send(sfd,buffer,strlen(buffer)+1,0);

        char buff[100];
        int n = recv(sfd,buff, 100,0);
        buff[n]='\0';

        cout<<buff<<endl;
    }

}