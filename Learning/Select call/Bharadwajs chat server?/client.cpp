#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sched.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<sys/un.h>
#include<csignal>
#include<string.h>
#include<bits/stdc++.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
using namespace std;
struct msg_hdr 
{
    long type;
    char text[100];
    long from;
};
int main() 
{
    key_t key = ftok("c.txt", 65);
    int pid = getpid();
    int msqid = msgget(key, 0777 | IPC_CREAT);
    cout<<key<<" "<<msqid<<"\n";
    struct msg_hdr message, message1;
    message.type = 1;
    message.from = pid;
    msgsnd(msqid, &message, sizeof(message), 0);
    while (1) 
    {
        fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(STDIN_FILENO, &read_fds); 
	int max_fd = STDIN_FILENO;  
	struct timeval timeout;
	timeout.tv_sec = 5; 
	timeout.tv_usec = 0;
        int result = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (result > 0 && FD_ISSET(STDIN_FILENO, &read_fds)) // Use STDIN_FILENO here
        {
            memset(message.text, 0, sizeof(message.text));
            fgets(message.text, sizeof(message.text), stdin);
            int n = strlen(message.text);
            message.text[n] = '\0';
            msgsnd(msqid, &message, sizeof(message), 0);
            memset(message.text, 0, sizeof(message.text));
            printf("Successfully sent ..\n");
        } 
        int k = msgrcv(msqid, &message1, sizeof(message1), pid, IPC_NOWAIT);
        if(k != -1) printf("%s\n", message1.text); 
    }
}