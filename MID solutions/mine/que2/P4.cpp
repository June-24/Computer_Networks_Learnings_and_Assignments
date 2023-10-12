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

int main()
{
    cout << "Enter the pid of the main S : ";
    int pid;
    cin >> pid;
    while (1)
    {
        cout << "Do you want to send signal now ? (yes/no): ";
        string s;
        cin >> s;
        if (s == "no")
            continue;
        else if (s == "yes")
        {
            kill(pid, SIGUSR1);
            cout << "sent signal to main S" << endl;
        }
    }
}