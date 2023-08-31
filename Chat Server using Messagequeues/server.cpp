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
    long msgtype;
    char msgtext[512];
};
int main ()
{
    // creating the msg queues
    int key_server = ftok("./server.txt" , 65);
    int key_client = ftok("./client.txt" , 65);
    int msqid_server = msgget(key_server , IPC_CREAT | 0666);
    int msqid_client = msgget(key_client , IPC_CREAT | 0666);
    map<int,int> mp;
    while (1)
    {   
        struct my_buffer temp;
        int t = msgrcv(msqid_server , &temp , sizeof(temp.msgtext) , 0 , 0);
        if (t!=-1)
        {
            // for first time, inserting into map
            if (temp.msgtype==1)
            {   
                cout<<"User#"<<temp.msgtext<<" joined the chat "<<endl;
                mp[stoi(temp.msgtext)] = 1;
                continue ;
            }
            // sending to all who are didnt send the message
            for (auto j : mp)
            {
                if (j.first!=temp.msgtype)
                {
                    struct my_buffer temp1;
                    temp1.msgtype = j.first;
                    strncpy(temp1.msgtext, temp.msgtext, sizeof(temp1.msgtext) - 1);
                    temp1.msgtext[sizeof(temp1.msgtext) - 1] = '\0';

                    int isfine = msgsnd(msqid_client, &temp1, strlen(temp1.msgtext) + 1, 0);
                    cout<<"Sent message .."<<endl;
                }
            }
        }
    }
}