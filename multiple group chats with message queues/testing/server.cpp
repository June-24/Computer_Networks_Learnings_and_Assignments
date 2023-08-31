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
int main()
{
    // creating the msg queues
    int key_server = ftok("./server.txt", 65);
    int key_client = ftok("./client.txt", 65);
    int msqid_server = msgget(key_server, IPC_CREAT | 0666);
    int msqid_client = msgget(key_client, IPC_CREAT | 0666);
    map<int, vector<int>> mp;
    while (1)
    {
        struct my_buffer temp;
        int t = msgrcv(msqid_server, &temp, sizeof(temp.data), 0, 0);
        if (t != -1)
        {
            // for first time, inserting into map
            if (temp.type == 1)
            {
                int n = temp.data[0] - '0';
                string str = "";
                int i = 0;
                for (i = 0; temp.data[i] != ':'; i++)
                {
                    str += temp.data[i];
                }
                i++;
                cout << "User#" << str << " joined the chat " << endl;
                temp.data[i + n] = '\0';
                for (int j = i; j < i + n; j++)
                {
                    mp[stoi(str)].push_back(temp.data[j] - '0');
                }
                cout << endl;
                continue;
            }
            // sending to all who are didnt send the message
            for (auto j : mp)
            {
                if (j.first!=temp.type)
                {
                    // need to send to people in that group
                    // cout<<temp.data<<endl;
                    int group_number = temp.data[0]-'0';
                    string str;
                    for(int i=2;i<strlen(temp.data);i++){
                        str+=temp.data[i];
                    }
                    for(auto x:j.second){
                        if(x==group_number){
                            cout<<"received from person# "<<temp.type<<" message is: "<<str<<endl;
                            cout<<"Sending to group number : "<<x<<" message is: "<<str<<endl;
                            struct my_buffer temp1;
                            temp1.type = j.first;
                            strncpy(temp1.data, temp.data, sizeof(temp1.data) - 1);
                            temp1.data[sizeof(temp1.data) - 1] = '\0';
                            int isfine = msgsnd(msqid_client, &temp1, strlen(temp1.data) + 1, 0);
                            cout<<"Sent message .."<<endl;
                        }

                    }
                }
            }
        }
    }
}