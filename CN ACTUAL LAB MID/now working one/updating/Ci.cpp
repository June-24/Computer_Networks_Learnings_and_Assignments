#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/msg.h>
#include <poll.h>
#include <csignal>
using namespace std;
// these ports contain the ports of Si's
vector<int> ports;
// count of all ports
int count_n = 0;
string name;

vector<int> sfds;
struct sockaddr_in sAddr, cAddr;
int adrlen = sizeof(sAddr);
int sfd = -1, msqid;
struct MyBuff
{
    long type;
    char pid[20];
};

int Get_Sfd_From_Old()
{
    cout << "My pid : " << getpid() << endl;
    int usfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    unlink("SendFD");
    strcpy(un.sun_path, "SendFD");

    if (bind(usfd, (struct sockaddr *)&un, sizeof(un)) < 0)
    {
        perror("usfd bind err");
        return 1;
    }
    char buf[512];
    struct iovec e = {buf, 512};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr m = {NULL, 0, &e, 1, cmsg, sizeof(cmsg), 0};
    if (recvmsg(usfd, &m, 0) < 0)
    {
        perror("usfd recvmsg err");
        exit(1);
    }
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    int sfd = *(int *)CMSG_DATA(c);
    return sfd;
}

void Send_Sfd_To_Next(int sig)
{
    sleep(1);
    int usfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un uAddr;
    uAddr.sun_family = AF_UNIX;
    strcpy(uAddr.sun_path, "SendFD");

    struct iovec e = {NULL, 0};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr m = {(void *)&uAddr, sizeof(uAddr), &e, 1, cmsg, sizeof(cmsg), 0};
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(c) = sfd;
    if (sendmsg(usfd, &m, 0) < 0)
    {
        perror("sendmsg err");
        exit(1);
    }
    cout << "sent sfd" << endl;
}

void Recv_From_ESS(int sig, siginfo_t *info, void *context)
{
    cout << "hello here";
    // sfd = -1;
    if (sfd == -1)
    {
        kill(info->si_pid, SIGUSR2);
        sfd = Get_Sfd_From_Old();
    }
    for(int i=0;i<5;i++)
    {
        char recvMsg[100] = {'\0'};
        recv(sfd, recvMsg, sizeof(recvMsg), 0);
        cout << recvMsg << endl;
        string sendMsg = "From " + name;
        send(sfd, sendMsg.c_str(), sizeof(sendMsg), 0);
    }
    MyBuff myMsg;
    myMsg.type = 1;
    strcpy(myMsg.pid, to_string(getpid()).c_str());
    msgsnd(msqid, &myMsg, sizeof(myMsg), 0);
    msgrcv(msqid, &myMsg, sizeof(myMsg), 0, 0);
    string pid = "";
    int sz = strlen(myMsg.pid);
    for (int i = 0; i < sz; i++)
        pid += myMsg.pid[i];
    kill(stoi(pid), SIGUSR1);
}
void listener()
{
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = Recv_From_ESS;
    sigaction(SIGUSR1, &sa, NULL);
    signal(SIGUSR2, Send_Sfd_To_Next);
    key_t key = ftok("./in2.txt", 65);
    msqid = msgget(111, 0666 | IPC_CREAT);
    MyBuff myMsg;
    myMsg.type = 1;
    strcpy(myMsg.pid, to_string(getpid()).c_str());
    msgsnd(msqid, &myMsg, sizeof(myMsg), 0);

    cout << "here now";
    struct pollfd pfds[sfds.size()];
    for (int i = 0; i < sfds.size(); i++)
    {
        pfds[i].fd = sfds[i];
        pfds[i].events = POLLIN;
    }
    while (1)
    {
        int r = poll(pfds, sfds.size(), 10);
        if (r > 0)
        {
            for (int i = 0; i < sfds.size(); i++)
            {
                if (pfds[i].revents & POLLIN)
                {
                    char msg[500];
                    int n = recv(pfds[i].fd, msg, sizeof(msg), 0);
                    if (n > 0)
                    {
                        msg[n] = '\0';
                        cout << msg << endl;
                    }
                }
            }
        }
    }
}
void print()
{

    cout << "Enter your name: ";
    cin >> name;
    cout << "how many Si do you want to connect ?(count)";
    cin >> count_n;
    cout << "Enter the ports of the Si you want to connect: " << endl;
    for (int i = 0; i < count_n; i++)
    {
        int j;
        cin >> j;
        ports.push_back(j);
    }
    cout << "the ports are: ";
    for (auto i : ports)
    {
        cout << i << " ";
    }
    cout << "conneting to them now" << endl;
    for (int i = 0; i < count_n; i++)
    {
        int sfddd;
        if ((sfddd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Error in socket()");
        }
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(ports[i]);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (connect(sfddd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            perror("Error in connect");
        }
        cout << "Connection successfull to port number : " << ports[i] << endl;
        sfds.push_back(sfddd);
    }
}
int main()
{
    cout << "My pid : " << getpid() << endl;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error in socket()");
        return 0;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "Someone already connected to ESS, you need to wait till the process releases: " << endl;
        print();
        listener();
        cout << "here now after connection" << endl;
    }
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = Recv_From_ESS;
    sigaction(SIGUSR1, &sa, NULL);
    signal(SIGUSR2, Send_Sfd_To_Next);
    while (1)
        ;
}