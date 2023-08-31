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
    // creating the message queues
    int key1 = ftok("./server.txt" , 65);
    int key2 = ftok("./client.txt" , 65);
    int server_msqid = msgget(key1 , IPC_CREAT | 0666);
    int client_msqid = msgget(key2 , IPC_CREAT | 0666);
    map<int,vector<int>> mp;
    while (1)
    {   
        struct my_buffer* temp = new my_buffer();
        int t = msgrcv(server_msqid , temp , sizeof(temp->data) , 0 , 0);
        if (t!=-1)
        {
            if (mp.find(temp->type)==mp.end())
            {   
                cout<<"User with UserId# "<<temp->data<<" joined the server "<<endl;
                int n=temp->data[0]-'0';
                vector<int> grpsin(26,0);
                for (int i=1;i<strlen(temp->data);i++)
                    grpsin[temp->data[i]-'A'] = 1;
                cout<<"And Belongs in the following groups : ";
                for (int i=0;i<26;i++){
                    if (grpsin[i]==0)
                        continue;
                    cout<<char(i+'A')<<' ';
                }
                cout<<endl;
                mp[temp->type] = grpsin;
                continue ;
            }
            cout<<"message recieved is : "<<temp->data<<endl;
            char grp = temp->data[0];
            for (auto members : mp)
            {
                // sending in only those who didnt send and are in that group
                if (members.first!=temp->type)
                {
                    // checking if in group or not
                    if (members.second[grp-'A']!=1)
                        continue;
                    struct my_buffer temp2;
                    temp2.type = members.first;
                    string str="UserId# "+to_string(temp->type)+" in group-> "+temp->data;
                    char const* group_data = (str).c_str();
                    strncpy(temp2.data,group_data,strlen(group_data));
                    temp2.data[strlen(group_data)] = '\0';
                    int isfine = msgsnd(client_msqid , &temp2 , strlen(temp2.data)+1 , 0);
                }
            }
        }
    }
}