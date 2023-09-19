#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<pthread.h>
#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <sched.h>
#include <sys/wait.h>
using namespace std;
unordered_set<int> st;
vector<string> split(const string &str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
int receive(void* args)
{
    int nsfd=*((int*)args);
    while(1)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int n=recv(nsfd,buffer,sizeof(buffer),0);
        int wfd=open("a.txt",O_RDONLY);
        if(n > 0)
        {
              vector<string> temp;
              char buffer[1000];
              read(wfd,buffer,sizeof(buffer));
              temp=split(buffer,' ');
                for(int i=0;i<temp.size();i++)
                {
                     int fd=atoi(temp[i].c_str());
                     if(fd!=nsfd)
                     {
                            send(fd,buffer,sizeof(buffer),0);
                     }
                }

             
        }
    }
}
int main()
{
     void *memory = malloc(1024*1024);
     int sfd=socket(AF_INET,SOCK_STREAM,0);
     struct sockaddr_in add;
     add.sin_family = AF_INET;
     add.sin_port = htons(1351);
     add.sin_addr.s_addr =inet_addr("127.0.0.1");
     int reuse = 1;
     setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
     bind(sfd,(struct sockaddr*)&add,sizeof(add));
     listen(sfd,100);
     int wfd=open("a.txt",O_WRONLY|O_APPEND);
     while(1)
     {
          int nsfd=accept(sfd,NULL,NULL);
          string g=to_string(nsfd);
          g.push_back(' ');
          const char* buffer=g.c_str();
          write(wfd,buffer,g.size());
          st.insert(nsfd);
          int *args=new int;
          *args=nsfd;
          clone(receive,memory+1024*1024, SIGCHLD,args);
     }
     close(sfd);
}
