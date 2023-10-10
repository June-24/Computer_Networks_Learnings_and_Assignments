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
using namespace std;
#define path "p3"
int getfd()
{
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
    struct sockaddr_un myAddr;
    myAddr.sun_family=AF_UNIX;
    strcpy(myAddr.sun_path,path);
    unlink(path);
    if(bind(usfd,(struct sockaddr*)&myAddr,sizeof(myAddr))<0)
    {
        perror("error in usfd binding");
        exit(EXIT_FAILURE);
    }
    struct iovec ioVector;
    ioVector.iov_base=NULL;     ioVector.iov_len=0;
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr socket_msg;
    socket_msg.msg_name=NULL;           socket_msg.msg_namelen=0;
    socket_msg.msg_iov=&ioVector;       socket_msg.msg_iovlen=1;
    socket_msg.msg_control=cmsg;        socket_msg.msg_controllen=sizeof(cmsg);
    socket_msg.msg_flags=0;
    cout<<"Waiting to recv fd..."<<endl;
    if(recvmsg(usfd,&socket_msg,0)<0)
    {
        perror("error in recvmsg system call");
        exit(EXIT_FAILURE);
    }
    struct cmsghdr *c=CMSG_FIRSTHDR(&socket_msg);
    return *(int*)CMSG_DATA(c);
}
void* service(void* arg)
{
        while(1)
        {
                int nsfd=*((int*)arg);
		char msg[100];
		int n=read(nsfd,msg,sizeof(msg));
		if(n <=0) continue;
		int sum=0;
		for(int i=0;i<strlen(msg);i++) sum+=msg[i]-'0';
		char temp[100];
		string s=to_string(sum);
		cout<<s<<"\n";
		for(int i=0;i<s.size();i++) temp[i]=s[i];
		write(nsfd,temp,s.size());
        }
}
int main()
{
    while(1)
    {
       int fd=getfd();
       pthread_t id;
       pthread_create(&id,NULL,&service,(void*)&fd);
    }
}
