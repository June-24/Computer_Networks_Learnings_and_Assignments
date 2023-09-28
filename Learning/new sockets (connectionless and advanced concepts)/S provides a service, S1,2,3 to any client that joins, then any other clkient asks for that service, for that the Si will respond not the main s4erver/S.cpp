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

// service S1
void* S1(void* arg){
    // getting sfd
    int sfd=*(int*)arg;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buffer[1024];
    // for first time check->
    bool flag=0;
    while(1){
        int n = recvfrom(sfd, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        getpeername(sfd, (struct sockaddr *)&client, &len);
        
        if(n>0){
            flag=1;
            buffer[n] = '\0';
            cout<<endl;
            cout<<"client port: "<<ntohs(client.sin_port)<<"   client IP  : "<<inet_ntoa(client.sin_addr)<<endl; 
            cout<<"1st and last time from S for S1: "<<buffer<<endl<<endl;
        }
        if(flag)
            break;
    }
    int c=0;
    c=fork();
    if(c==0){
        dup2(sfd,1);
        char* path[]={"./S1",NULL};
        execvp(path[0],path);
    }
}
// service S2
void* S2(void* arg){
    // getting sfd
    int sfd=*(int*)arg;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buffer[1024];
    // for first time check->
    bool flag=0;
    while(1){
        int n = recvfrom(sfd, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        getpeername(sfd, (struct sockaddr *)&client, &len);
        
        if(n>0){
            flag=1;
            buffer[n] = '\0';
            cout<<endl;
            cout<<"client port: "<<ntohs(client.sin_port)<<"   client IP  : "<<inet_ntoa(client.sin_addr)<<endl; 
            cout<<"1st and last time from S for S2: "<<buffer<<endl<<endl;
        }
        if(flag)
            break;
    }
    int c=0;
    c=fork();
    if(c==0){
        dup2(sfd,1);
        char* path[]={"./S2",NULL};
        execvp(path[0],path);
    }
}
// service S3
void* S3(void* arg){
    // getting sfd
    int sfd=*(int*)arg;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buffer[1024];
    // for first time check->
    bool flag=0;
    while(1){
        int n = recvfrom(sfd, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        getpeername(sfd, (struct sockaddr *)&client, &len);
        
        if(n>0){
            flag=1;
            buffer[n] = '\0';
            cout<<endl;
            cout<<"client port: "<<ntohs(client.sin_port)<<"   client IP  : "<<inet_ntoa(client.sin_addr)<<endl; 
            cout<<"1st and last time from S for S3: "<<buffer<<endl<<endl;
        }
        if(flag)
            break;
    }
    int c=0;
    c=fork();
    if(c==0){
        dup2(sfd,1);
        char* path[]={"./S3",NULL};
        execvp(path[0],path);
    }
}
// service S4
void* S4(void* arg){
    // getting sfd
    int sfd=*(int*)arg;
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buffer[1024];
    // for first time check->
    bool flag=0;
    while(1){
        int n = recvfrom(sfd, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        getpeername(sfd, (struct sockaddr *)&client, &len);
        
        if(n>0){
            flag=1;
            buffer[n] = '\0';
            cout<<endl;
            cout<<"client port: "<<ntohs(client.sin_port)<<"   client IP  : "<<inet_ntoa(client.sin_addr)<<endl; 
            cout<<"1st and last time from S for S4: "<<buffer<<endl<<endl;;
        }
        if(flag)
            break;
    }
    int c=0;
    c=fork();
    if(c==0){
        dup2(sfd,1);
        char* path[]={"./S4",NULL};
        execvp(path[0],path);
    }
}
int main(){
    // server using connection less socket
    int sfd[4]={0};
    int ports[4]={8090,8091,8092,8093};
    for(int i=0;i<4;i++){
        sfd[i] = socket(AF_INET, SOCK_DGRAM, 0);
        if(sfd[i] == -1){
            cout<<"Error in creating socket"<<endl;
            exit(1);
        }
        cout<<"Socket created successfully"<<endl;
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(ports[i]);
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        bind(sfd[i], (struct sockaddr *)&server, sizeof(server));
    }

    // create 4 threads to handle 4 different ports
    pthread_t tid[4];
    pthread_create(&tid[0], NULL, &S1, &sfd[0]);
    pthread_create(&tid[1], NULL, &S2, &sfd[1]);
    pthread_create(&tid[2], NULL, &S3, &sfd[2]);
    pthread_create(&tid[3], NULL, &S4, &sfd[3]);

    for(int i=0;i<4;i++){
        pthread_join(tid[i], NULL);
    }
    

}