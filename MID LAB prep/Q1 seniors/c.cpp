#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h> 
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sched.h>
#include<sys/wait.h>
#include<netinet/in.h>  
#include<sys/un.h>
#include<csignal>
#include<bits/stdc++.h>
#include<sys/shm.h>
using namespace std;
#define port1 2011
#define port2 3011
#define port3 4011
#define port4 5011
void create_socket(int &sfd)
{
    if((sfd=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("error in socket system call");
        exit(1);
    }
}
void connect_socket(int &sfd,int service)
{
    struct sockaddr_in add;
    int adrlen=sizeof(add);
    add.sin_family=AF_INET;
    add.sin_addr.s_addr=INADDR_ANY;
    int port=(service == 1)?(port1):(service == 2)?(port2):(service == 3)?(port3):(port4);
    add.sin_port=htons(port);
    cout<<port<<endl;
    connect(sfd,(struct sockaddr*)&add,sizeof(add)); 
}
void take_service(int arr[],int sfd)
{
    char msg[100],ans[100];
    memset(msg, 0, sizeof(msg));
    memset(ans, 0, sizeof(ans));
    for(int i=0;i<5;i++) msg[i]=char(arr[i]+48);
    cout<<"Input taken succesfully..\n";
    write(sfd,msg,sizeof(msg));
    cout<<"Input wrote succesfully..\n";
    read(sfd,ans,sizeof(ans));
    cout<<"Sum of numbers = "<<ans<<"\n";
    fflush(stdout);
    memset(msg, 0, sizeof(msg));
    memset(ans, 0, sizeof(ans));
}
void take_input(int arr[])
{
    cout<<"Enter 5 numbers between 0 and 9\n";
    for(int i=0;i<5;i++) cin>>arr[i];
}
int main()
{
    int service;
    cout<<"Enter which service you want \n";
    cin>>service;
    int sfd;
    create_socket(sfd);
    connect_socket(sfd,service);
    while(1) 
    {
       int arr[5];
       take_input(arr);
       take_service(arr,sfd);
    }
}
