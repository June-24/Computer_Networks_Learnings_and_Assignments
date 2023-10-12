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
    mkfifo("P22", 0666);
    int fd1 = open("P22", O_WRONLY | O_NONBLOCK);
    while (1)
    {
        cout << "Enter a message for S: ";
        string s;
        getline(cin, s);
        const char *t = s.c_str();
        write(fd1, t, sizeof(t));
        sleep(3);
    }
}