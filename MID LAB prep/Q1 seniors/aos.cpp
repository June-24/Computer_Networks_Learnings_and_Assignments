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
#include<pthread.h>
#include<poll.h>
using namespace std;
#define port1 2011
#define port2 3011
#define port3 4011
#define port4 5011
#define path1 "p1"
#define path2 "p2"
#define path3 "p3"
#define path4 "p4"
void send_fd(int sfd,int id)
{
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    struct sockaddr_un myAddr;
    myAddr.sun_family=AF_UNIX;
    if(id == 1) strcpy(myAddr.sun_path,path1);
    if(id == 2) strcpy(myAddr.sun_path,path2);
    if(id == 3) strcpy(myAddr.sun_path,path3);
    if(id == 4) strcpy(myAddr.sun_path,path4);
    struct iovec ioVector;
    ioVector.iov_base=NULL;     
    ioVector.iov_len=0;
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr socket_msg;
    socket_msg.msg_name=(void*)&myAddr;     socket_msg.msg_namelen=sizeof(myAddr);
    socket_msg.msg_iov=&ioVector;           socket_msg.msg_iovlen=1;
    socket_msg.msg_control=cmsg;            socket_msg.msg_controllen=sizeof(cmsg);
    socket_msg.msg_flags=0;
    struct cmsghdr *c = CMSG_FIRSTHDR(&socket_msg);
    c->cmsg_level=SOL_SOCKET;   c->cmsg_type=SCM_RIGHTS;
    c->cmsg_len=CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(c) = sfd;
    if(sendmsg(usfd, &socket_msg, 0)<0)
    {
        perror("error in sendmsg system call");
        exit(1);
    }
}
void send_signal()
{
   key_t key=ftok("a.txt",65);
   int shmid=shmget(key,1024,0666|IPC_CREAT);
   int* shm=(int*)shmat(shmid,(void*)0,0);   
   kill(*shm,SIGUSR1);
}
int main()
{
    int sfd1;
    if((sfd1=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("error in socket system call");
        exit(1);
    }
    int opt=1;
    if(setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0)
    {
        perror("error in setsockopt system call");
        exit(1);
    }
    struct sockaddr_in add1;
    add1.sin_family = AF_INET;
    add1.sin_port = htons(port1);
    add1.sin_addr.s_addr =inet_addr("127.0.0.1");
    if(bind(sfd1,(struct sockaddr*)&add1,sizeof(add1)) == -1)
    {
       perror("error in bind system call");
       exit(0);
    }
    if(listen(sfd1,100) == -1)
    {
       perror("error in listen system call");
       exit(0);
    }
    
    
    
    int sfd2;
    if((sfd2=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("error in socket system call");
        exit(1);
    }
    opt=1;
    if(setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0)
    {
        perror("error in setsockopt system call");
        exit(1);
    }
    struct sockaddr_in add2;
    add2.sin_family = AF_INET;
    add2.sin_port = htons(port2);
    add2.sin_addr.s_addr =inet_addr("127.0.0.1");
    if(bind(sfd2,(struct sockaddr*)&add2,sizeof(add2)) == -1)
    {
       perror("error in bind system call");
       exit(0);
    }
    if(listen(sfd2,100) == -1)
    {
       perror("error in listen system call");
       exit(0);
    }
    
    
    
    int sfd3;
    if((sfd3=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("error in socket system call");
        exit(1);
    }
    opt=1;
    if(setsockopt(sfd3, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0)
    {
        perror("error in setsockopt system call");
        exit(1);
    }
    struct sockaddr_in add3;
    add3.sin_family = AF_INET;
    add3.sin_port = htons(port3);
    add3.sin_addr.s_addr =inet_addr("127.0.0.1");
    if(bind(sfd3,(struct sockaddr*)&add3,sizeof(add3)) == -1)
    {
       perror("error in bind system call");
       exit(0);
    }
    if(listen(sfd3,100) == -1)
    {
       perror("error in listen system call");
       exit(0);
    }
    
    
    
    
    int sfd4;
    if((sfd4=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("error in socket system call");
        exit(1);
    }
    opt=1;
    if(setsockopt(sfd4, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))<0)
    {
        perror("error in setsockopt system call");
        exit(1);
    }
    struct sockaddr_in add4;
    add4.sin_family = AF_INET;
    add4.sin_port = htons(port4);
    add4.sin_addr.s_addr =inet_addr("127.0.0.1");
    if(bind(sfd4,(struct sockaddr*)&add4,sizeof(add4)) == -1)
    {
       perror("error in bind system call");
       exit(0);
    }
    if(listen(sfd4,100) == -1)
    {
       perror("error in listen system call");
       exit(0);
    }
    send_fd(sfd4,4);
    
    
    struct pollfd pfd[4];
    int sfd[4]={sfd1,sfd2,sfd3,sfd4};
    for(int i=0;i<4;i++)
    {
       pfd[i].fd=sfd[i];
       pfd[i].events=POLLIN;
    }
    while(1)
    {
         int ret=poll(pfd,4,5);
         if(ret > 0)
         {
             for(int i=0;i<4;i++)
             {
              if(pfd[i].revents & POLLIN)
              {
                       if(i == 3)
                       {
                           cout<<"Signal sent\n";
                           send_signal();
                       }
                       else
                       {
                               int nsfd;
			       if((nsfd = accept(pfd[i].fd,NULL,NULL)) == -1)
			       {
				   perror("error in accept system call");
				   exit(0);
			       }
			       cout<<nsfd<<" "<<i+1<<"\n";
			       send_fd(nsfd,i+1); 
                       }
              }
             }
         }
    }
}
