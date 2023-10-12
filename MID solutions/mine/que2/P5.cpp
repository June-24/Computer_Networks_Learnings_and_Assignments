#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

int main ()
{
    int c=0;
    c=fork();
    if(c>0){
        cout<<"parent";

    }
    else{
        cout<<"Cild";
    }
}