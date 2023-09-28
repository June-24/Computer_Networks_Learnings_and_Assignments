#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main (){
    
    // cout<<"Which Service do you want ? : \n";
    // cout<<"1. Addition\n2. Subraction"<<endl;
    
    // int i; cin>>i;

    // the four ports
    vector<int> ports = {50001,50002,50003,50004};

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
    cout<<"Enter number to add : "<<endl;
    getline(cin,s);

    const char * buffer = s.c_str();
    send(sfd,buffer,strlen(buffer)+1,0);

    char buff[100];
    int n = recv(sfd, buff, 100,0);
    cout<<n<<endl;
    buff[n]='\0';

    cout<<"Answer is : "<<buff<<endl;
}