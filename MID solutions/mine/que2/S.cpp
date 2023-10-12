#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <string.h>

using namespace std;

vector<int> queuee;

void func(int signo)
{
    int c = fork();

    if (c == 0)
    {
        sleep(1);
        int ifd = queuee[0];
        dup2(ifd, 1);
        sleep(0.5);
        execl("./E", "./E", NULL); 
    }
    else
    {
        close(queuee[0]);
        sleep(1);
        cout << queuee.size() << endl;
        cout<<"first element of queue popped"<<endl;
        queuee.erase(queuee.begin());
        cout << queuee.size() << endl;
    }
}

int main()
{
    cout << "Server S pid :  " << getpid() << endl;
    int c = 0;
    int pp[2];
    pipe(pp);
    if (fork() > 0)
    {
        close(pp[1]);
    }
    else
    {
        cout << "child";
        fflush(stdout);
        close(pp[0]);
        dup2(pp[1], 1);
        execl("./P1", "./P1", NULL); 
    }

    cout << "P1 initialised " << endl;
    fflush(stdout);

    mkfifo("P22", 0666);
    int ffd = open("P22", O_RDONLY | O_NONBLOCK, 0666);
    cout << "mkfifo initialised" << endl;
    fflush(stdout);

    int fd1 = fileno(popen("./P3", "r"));
    cout << "popen done" << endl;
    fflush(stdout);
    // signal
    signal(SIGUSR1, func);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        cout << "Error in creating socket: " << strerror(errno) << endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(8082);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind(sfd, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (binderror == -1)
    {
        cout << "Error in binding: " << strerror(errno) << endl;
        return 0;
    }

    int listenerror = listen(sfd, 10);
    if (listenerror == -1)
    {
        cout << "Listening error is found: " << strerror(errno) << endl;
        return 0;
    }
    cout << "socket done" << endl;
    struct pollfd pfd[5];
    pfd[0].fd = 0;
    pfd[1].fd = pp[0];
    pfd[2].fd = ffd;
    pfd[3].fd = fd1;
    pfd[4].fd = sfd;
    for (int i = 0; i < 5; i++)
    {
        pfd[i].events = POLLIN;
    }

    cout << "poll initialised" << endl;
    fflush(stdout);

    while (1)
    {
        int s = poll(pfd, 5, 10);
        if (s <= 0)
            continue;

        for (int i = 0; i < 5; i++)
        {
            if (i == 4 && (pfd[4].revents & POLLIN))
            {
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int newfd = accept(sfd, (struct sockaddr *)&clientaddr, &len);
                if (newfd == -1)
                {
                    cout << "accept error: " << strerror(errno) << endl;
                    fflush(stdout);

                    continue;
                }
                cout << "New client accepted" << endl;
                fflush(stdout);

                queuee.push_back(newfd);
            }
            else if (pfd[i].revents & POLLIN)
            {
                char buffer[100];
                int n = read(pfd[i].fd, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    sleep(1);
                    buffer[n] = '\0';
                    fflush(stdout);
                    for (auto j : queuee)
                    {
                        send(j, buffer, strlen(buffer), 0); 
                    }
                }
            }
        }
    }
}
