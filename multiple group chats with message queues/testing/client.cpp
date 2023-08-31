#include <bits/stdc++.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

struct my_buffer 
{
    long type;
    char data[512];
};

int main ()
{
    // creating the msg queues
    int key_server = ftok("./server.txt" , 65);
    int key_client = ftok("./client.txt" , 65);
    int msqid_server = msgget(key_server , IPC_CREAT | 0666);
    int msqid_client = msgget(key_client , IPC_CREAT | 0666);
    int n;
    cout<<"enter the number of groups youre in: ";
    cin>>n;
    cout<<"Enter group numbers that you belong in: ";
    string str="",str1;
    for(int i=0;i<n;i++){
        cin>>str1;
        str=str+str1;
    }
    // sending teh pid first to server
    long pid = (long)getpid();
    struct my_buffer sending_pid;
    sending_pid.type = 1;//first time is 1
    string msg=to_string(n)+to_string(getpid())+":"+str;
    cout<<msg<<endl;
    char const* pidd = (msg).c_str();
    strncpy(sending_pid.data,pidd,strlen(pidd));
    sending_pid.data[strlen(pidd)] = '\0';
    msgsnd(msqid_server , &sending_pid , strlen(sending_pid.data) , 0);// sending the pid to server

    // polling the keyboard
    struct pollfd pfd[1];
    pfd[0].fd = 0; pfd[0].events = POLLIN;
    while (1)
    {
        int s = poll(pfd , 1 , 10);
        if (s>0 && pfd[0].revents & POLLIN)
        {
            struct my_buffer temp;
            string str;
            getline(cin,str);
            
            for(int i=0;i<str.length();i++){
                temp.data[i]=str[i];
            }
            temp.data[str.length()]='\0';
            temp.type = pid;
            msgsnd(msqid_server , &temp , strlen(temp.data) , 0);
        }

        struct my_buffer temp1;
        int t = msgrcv(msqid_client , &temp1 , sizeof(temp1.data) , getpid() , MSG_NOERROR | IPC_NOWAIT);
        // cout<<t<<endl;
        if (t!=-1){
            temp1.data[t] = '\0';
            
            cout<<"User# "<<temp1.type<<" group #"<<temp1.data<<endl;
        }
        else{
            cout<<"smth";
        }
    }
}