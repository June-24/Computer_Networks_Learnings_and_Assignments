#include <bits/stdc++.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <poll.h>
using namespace std;

int main()
{
    string pidd = to_string(getpid());

    cout << "Joining process " << pidd << " in the chat room ...." << endl;
    cout << endl;

    char makepipe[pidd.length() + 1];
    for (int i = 0; i < pidd.length(); i++)
        makepipe[i] = pidd[i];
    makepipe[pidd.length()] = '\0';

    mkfifo(makepipe, 0666);

    const char *pid = pidd.c_str();

    int wfd = open("chattalks", O_WRONLY);

    write(wfd, pid, strlen(pid));

    int rfd = open(pid, O_RDONLY);

    struct pollfd pfd[1];
    pfd[0].fd = rfd;
    pfd[0].events = POLLIN;

    while (1)
    {
        int s = poll(pfd, 1, 1);
        if (s > 0)
        {
            char buffer[1024];
            if (pfd[0].revents & POLLIN)
            {
                read(rfd, buffer, sizeof(buffer));
                cout << buffer << endl;
            }
        }
    }
}
