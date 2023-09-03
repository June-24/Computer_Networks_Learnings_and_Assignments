#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

int main ()
{
    // opening the famous fifo now
    mkfifo ("ChatServer" , 0666);
    // map for keeping the pid and the ifd for each pid
    map<string,int> mp;
    
    int ffd = open("ChatServer" , O_RDONLY);

    while (1)
    {
        char buffer[1024] = {'\0'};
        read(ffd , buffer , 1024); 
        bool new_comer = true;
        string pid;
        for (int i=0;i<1024;i++)
        {
            if (buffer[i]==':')
            {
                // not first time joining so make it as new and break;
                new_comer = false;
                break;
            }
            else if (buffer[i]=='\0')
                break;
            else
                pid += buffer[i];
        }
        if (new_comer && mp.find(pid)==mp.end())
        {   
            for(auto i:mp){
                if (i.first!=pid)
                {  
                    string t=pid+" has joined the chat";
                    const char * arr = t.c_str();
                    write(mp[i.first] , arr , sizeof(arr));
                }
            }
            const char * arr = pid.c_str(); 
            int tfd = open(arr , O_WRONLY);
            mp[pid] = tfd;
        }
        else
        {   
            for (auto i : mp)
            {   
                if (i.first!=pid)
                {  
                    write(mp[i.first] , buffer , sizeof(buffer));
                }
            }
        }
    }
}