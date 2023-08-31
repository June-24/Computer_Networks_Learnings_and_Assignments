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
    mkfifo ("chattalks" , 0666);
    map<string,int> mp;
    
    int rdf = open("chattalks" , O_RDONLY);

    while (1)
    {
        char buffer[1024] = {'\0'};

        read(rdf , buffer , 1024); 

        bool joinnow = true;
        string pid;

        for (int i=0;i<1024;i++)
        {
            if (buffer[i]==' ')
            {
                joinnow = false;
                break;
            }
            else if (buffer[i]=='\0')
                break;
            else
                pid += buffer[i];
        }

        cout<<"The message recieved is "<<buffer<<endl;

        if (joinnow && !mp.count(pid))
        {   
            cout<<"New client : "<<buffer<<" joined the chat .."<<endl;
            
            const char * arr = pid.c_str(); 
            int tfd = open(arr , O_WRONLY);
            mp[pid] = tfd;
        }
        else
        {   
            for (auto i : mp)
            {    cout<<i.first<<endl;
                if (i.first!=pid)
                {  
                    cout<<i.first<<"  "<<pid<<endl;

                    write(mp[i.first] , buffer , sizeof(buffer));
                    cout<<"Written finally"<<endl;
                }
            }
        }
    }
}