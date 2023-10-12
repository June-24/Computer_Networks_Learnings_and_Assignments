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
// SFDi:port
vector<pair<int, int>> SFDi;
// SFD:port
map<pair<int, int>, int> Clients_Data;
// SFDi,pid
map<int, int> pidss;

int main()
{
    // connectionless SFD for getting data from any client
    int SFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (SFD == -1)
    {
        cout << "Error in creating socket" << endl;
        return 1;
    }
    cout << "Socket created successfully" << endl;

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(8080);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    int binderror = bind(SFD, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (binderror == -1)
    {
        cout << "Error in binding " <<endl<<endl<<endl;
        return 0;
    }
    // poll part
    struct pollfd pfd[100];
    pfd[0].fd = 0;
    pfd[0].events = POLLIN;
    int cnt = 1;

    while (1)
    {
        int s = poll(pfd, cnt, 100);

        if (s <= 0)
            continue;
        for (int i = 0; i < cnt; i++)
        {
            if (i == 0 && (pfd[0].revents & POLLIN))
            {
                // user input
                int s = 0;
                string str1, str2;
                cin >> str1 >> str2; // port , path ...

                cout << str1 << " " << str2 << endl;
                int sfd = socket(AF_INET, SOCK_STREAM, 0);
                if (sfd == -1)
                {
                    cout << "Error in creating socket" << endl;
                    return 1;
                }

                int reuse = 1;
                if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0)
                    perror("setsockopt(SO_REUSEADDR) failed");

                if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (const char *)&reuse, sizeof(reuse)) < 0)
                    perror("setsockopt(SO_REUSEPORT) failed");

                struct sockaddr_in serveraddress;
                serveraddress.sin_family = AF_INET;
                serveraddress.sin_port = htons(stoi(str1));
                serveraddress.sin_addr.s_addr = INADDR_ANY;

                int binderror = bind(sfd, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
                if (binderror == -1)
                {
                    cout << "Error in binding " << endl;
                    return 0;
                }

                int listenerror = listen(sfd, 10);
                if (listenerror == -1)
                {
                    cout << "Listening error is found " << endl;
                    return 0;
                }

                SFDi.push_back({sfd, stoi(str1)});
                pfd[cnt].fd = sfd;
                pfd[cnt].events = POLLIN;
                cnt++;
                string sending = "latest SFD, path is :  " + str2 + "  and port is :  " + str1;
                const char *buf = sending.c_str();
                // from the map sending to all others
                for(auto mp:Clients_Data)
                {
                    if(mp.second==1)
                    {
                        struct sockaddr_in clientaddress;
                        clientaddress.sin_family = AF_INET;
                        clientaddress.sin_port = mp.first.second;
                        clientaddress.sin_addr.s_addr = INADDR_ANY;
                        sendto(SFD, buf, strlen(buf) + 1, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
                    }
                }
                int c = 0;
                c = fork();
                pidss[sfd] = c;
                if (c == 0)
                {
                    const char *exefile = str2.c_str();
                    dup2(sfd, 1);
                    execv(exefile, NULL);
                }
                else
                {
                    sleep(2);
                }
            }
            else
            {
                // client is pinging on the sfdi, so we send the signsl to that si
                if (pfd[i].revents & POLLIN)
                {
                    cout<<"sending signal to that Si to which the client asked: "<<endl<<endl<<endl;
                    // here the pid we cannt take input need to save that
                    // from the i==1 code part before exec we get the child pid need to use that
                    int pid=pidss[pfd[i].fd];
                    kill(pid, SIGUSR1);
                }
            }
        }
        // getting data of any message from any client
        struct sockaddr_in clientaddress;
        char buffer[100];
        socklen_t client_len = sizeof(clientaddress);
        int n = recvfrom(SFD, &buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&clientaddress, &client_len);
        getpeername(SFD, (struct sockaddr *)&clientaddress, &client_len);
        Clients_Data[{SFD, clientaddress.sin_port}] = 1;
        if (n > 0)
        {
            buffer[n] = '\0';

            // need to send data of all the sfds to the clients
            string sending = "All SFDs and Ports are : ";
            for (int i = 0; i < SFDi.size(); i++)
            {
                sending += to_string(SFDi[i].first);
                sending += ":";
                sending += to_string(SFDi[i].second);
                sending += " , ";
            }
            // sending the data back to the client who requested
            const char *buf = sending.c_str();
            sendto(SFD, buf, strlen(buf) + 1, 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
        }
    }
}