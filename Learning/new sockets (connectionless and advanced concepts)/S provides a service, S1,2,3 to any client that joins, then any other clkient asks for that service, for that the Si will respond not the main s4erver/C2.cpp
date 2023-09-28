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
    // client uisng connectionless socket
    int sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sfd == -1){
        cout<<"Error in creating socket"<<endl;
        exit(1);
    }
    cout<<"Socket created successfully"<<endl;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8091);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    while(1){
        string s;
        cout<<"enter a message for service S2: ";
        getline(cin, s);
        const char *s1 = s.c_str();
        sendto(sfd, s1, strlen(s1), 0, (struct sockaddr *)&server, sizeof(server));
        // sleep(1);
    }
}