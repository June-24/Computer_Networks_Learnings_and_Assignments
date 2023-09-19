#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include<sys/stat.h>

using namespace std;

int main()
{
    int fd1 = open("./in1.txt", O_RDONLY);
    int fd2 = open("./in2.txt", O_RDONLY);
    int fd3 = open("./in3.txt", O_RDONLY);
    
    struct pollfd pfds[3];
    pfds[0].fd = 0;
    pfds[0].events = POLLIN;

    pfds[1].fd = fd2;
    pfds[1].events = POLLIN;

    pfds[2].fd = fd3;
    pfds[2].events = POLLIN;


    while (1)
    {
        int ret = poll(pfds, 3, 10);
        if (ret > 0)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (pfds[i].revents & POLLIN)
                {
                    char buffer[256];
                    int bytesRead = read(pfds[i].fd, buffer, sizeof(buffer));
                    if (bytesRead > 0)
                    {
                        buffer[bytesRead] = '\0';
                        cout << "Read from fd " << pfds[i].fd << ": " << buffer << endl;
                    }
                }
            }
        }
    }

    return 0;
}
