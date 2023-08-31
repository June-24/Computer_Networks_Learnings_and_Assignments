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
    char data[512] = {'\0'};
};
int main ()
{
    // creatig the message queues
    int key1 = ftok("./server.txt" , 65);
    int key2 = ftok("./client.txt" , 65);
    int server_msqid = msgget(key1 , IPC_CREAT | 0666);
    int client_msqid = msgget(key2 , IPC_CREAT | 0666);

    // first message to server details about the group
    long pid = (long)getpid();
    cout<<"Enter the name of groups (A,B...Z): (capital alphabets)"<<endl;
    string groups; 
    getline(cin,groups);
    groups=to_string(groups.length())+groups;
    struct my_buffer sending_pid;
    sending_pid.type = pid;
    char const* group_data = (groups).c_str();
    strncpy(sending_pid.data,group_data,strlen(group_data));
    sending_pid.data[strlen(group_data)] = '\0';
    msgsnd(server_msqid , &sending_pid , strlen(sending_pid.data)+1 , 0);

    // polling the keyboard
    struct pollfd pfd[1];
    pfd[0].fd = 0; pfd[0].events = POLLIN;
    while (1)
    {
        int s = poll(pfd , 1 , 100);
        if (s > 0 && pfd[0].revents & POLLIN)
        {
            struct my_buffer temp;
            fgets(temp.data , 100 , stdin);
            if (groups.find(temp.data[0])==string::npos)
            {
                cout<<"Cant send because you dont belong in that group"<<endl;
                continue;
            }
            temp.type = pid;
            temp.data[strlen(temp.data)] = '\0';
            int isfine = msgsnd(server_msqid , &temp , strlen(temp.data)+1 , 0);
        }
        struct my_buffer temp2;
        int result = msgrcv(client_msqid , &temp2 , sizeof(temp2.data) , getpid() , MSG_NOERROR | IPC_NOWAIT);
        if (result!=-1)
            cout<<temp2.data<<endl;
    }
}