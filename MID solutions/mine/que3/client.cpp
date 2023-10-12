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
int sfd = 0;
struct sockaddr_in serveraddress;
void *sendd(void *args)
{
    while (1)
    {
        cout << "Enter 1 for getting enquiry and 2 for connecting to any port " << endl;
        string choice;
        cin >> choice;
        if (choice == "1")
        {
            string str = choice;
            const char *buf = str.c_str();
            cout << "sending the text : " << str << endl;
            fflush(stdout);
            sendto(sfd, buf, strlen(buf), 0, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
        }
        else
        {
            cout << "Enter the port number to connect to" << endl;
            int port;
            cin >> port;
            int sfdd = socket(AF_INET, SOCK_STREAM, 0);
            if (sfdd == -1)
            {
                cout << "Error in creating socket" << endl;
                return NULL;
            }
            cout << "Socket created successfully" << endl;
            struct sockaddr_in serveraddresss;
            serveraddresss.sin_family = AF_INET;
            serveraddresss.sin_port = htons(port);
            serveraddresss.sin_addr.s_addr = inet_addr("127.0.0.1");
            int con = connect(sfdd, (struct sockaddr *)&serveraddresss, sizeof(serveraddresss));
            if (con == -1)
            {
                cout << "Error in connecting" << endl;
                return NULL;
            }
            cout << "Connected successfully" << endl;
            fflush(stdout);

            while (1)
            {
                string str1;
                cout << "Enter the message for the Si: ";
                getline(cin, str1);
                const char *buf = str1.c_str();
                cout << "sending the text : " << str1 << endl;
                fflush(stdout);
                send(sfdd, buf, strlen(buf), 0);
            }
        }
    }
}
void *recvv(void *args)
{
    while (1)
    {
        char buf[100];
        socklen_t len;
        int n = recvfrom(sfd, buf, 100, 0, (struct sockaddr *)&serveraddress, &len);
        if (n > 0)
        {
            buf[n] = '\0';
            cout << buf << endl;
            fflush(stdout);
        }
    }
}
int main()
{                       //dgram
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1)
    {
        cout << "Error in creating socket" << endl;
        return 1;
    }
    cout << "Socket created successfully" << endl;

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(8080);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sfd, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    cout << "binded" << endl;

    pthread_t sendd_thread, recvv_thread;
    pthread_create(&sendd_thread, NULL, sendd, NULL);
    pthread_create(&recvv_thread, NULL, recvv, NULL);

    pthread_join(sendd_thread, NULL);
    pthread_join(recvv_thread, NULL);
}