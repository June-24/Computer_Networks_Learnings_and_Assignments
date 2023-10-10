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
#include<string.h>
#include<bits/stdc++.h>
#include<sys/types.h>  
#include<sys/ipc.h>    
#include<sys/msg.h> 
using namespace std;
unordered_set<int> st;
struct msg_hdr
{
    long type;
    char text[100];
    long from;
};
int main()
{
    key_t key=ftok("c.txt",65);
    int msqid=msgget(key,0777|IPC_CREAT);
    cout<<msqid<<"\n"<<key<<"\n";
    struct msg_hdr message;
    while(1)
    {
       int k=msgrcv(msqid,&message,sizeof(message),1,0);
       if(k != -1) 
       {
                int y=message.from;
                if(st.count(y)==false) st.insert(y);
                for(auto i=st.begin();i != st.end();i++)
                {
                    if(*i != y)
                    {
                        message.type=*i;
                        msgsnd(msqid,&message,sizeof(message),0);
                    }
                }
       }
    }
}