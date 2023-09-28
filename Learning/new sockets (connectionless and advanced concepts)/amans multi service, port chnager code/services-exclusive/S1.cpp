#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/poll.h>
#include<pthread.h>
#include <unistd.h>

using namespace std;

//The structure to send fd to service
struct store{
    int fd;
};

//----------------------------------------------------------------------
//                  Funtion to service clients
//----------------------------------------------------------------------


void * service1(void* _arg){

    cout<<"reached here"<<endl;

    struct store* arg = (struct store*) _arg;

    char buffer[100];
    int n = recv(arg->fd, buffer,100,0);

    if(n>=0) buffer[n]='\0';
    cout<<buffer<<endl;
    cout<<"here"<<endl;

    stringstream ss(buffer);
    string a,b; 
    ss>>a; ss>>b;

    int num1 = stoi(a);
    int num2 = stoi(b);

    string answer = to_string(num1+num2);
    const char* buff = answer.c_str();

    send(arg->fd,buff, strlen(buff)+1,0);
    
    return NULL;
}

//------------------------------------------------------------
//------------------------------------------------------------



int main (){

    //----------------------------------------------------------
    //                      We first make sfds
    //----------------------------------------------------------

    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1) perror("Socket error");

    int reuse = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEPORT) failed");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(50001);
    address.sin_addr.s_addr = INADDR_ANY;

    //Bind
    int a = bind (sfd , (struct sockaddr *)&address , sizeof(address));
    if (a == -1) perror("Bind error S1");

    //Listen
    int b = listen(sfd , 10);
    if (b == -1) perror("Listen error");

    if(a == -1 || b == -1) return 0;

    cout<<" -- Service 1 up and running --"<<endl;

    //---------------------------------------------------------------------
    //Service who woke this service on the first place
    //Since out standard input points toward that we can extract it from there

    // Other options worth trying are : 
    // 1. Move the file descriptor through shared memory from the super server
    // 2. Extract it from the interrupt shit which i couln't find

    int first_client;
    dup2(0,first_client);
    {
        struct store* temp1 = new store();
        temp1->fd = first_client;
        cout<<"here"<<endl;
        pthread_t t;
        int temp = pthread_create(&t, NULL ,service1,temp1);
        pthread_join(t,NULL);
    }

    //---------------------------------------------------------------------

    while(1){

        cout<<"Now inside"<<endl;

        struct sockaddr * client_address;
        socklen_t len = sizeof(client_address);

        int nsfd = accept(sfd, (struct sockaddr*)&client_address , &len);

        cout<<"accepted  client"<<endl;

        // now here we need to spawn a new thread and sent this sfd in it

        struct store* temp1 = new store();
        temp1->fd = nsfd;
        pthread_t t;
        int temp = pthread_create(&t, NULL ,service1,temp1);
    }
}