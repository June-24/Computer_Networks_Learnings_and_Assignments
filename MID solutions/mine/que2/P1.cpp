#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sched.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
using namespace std;
int main()
{
    int fd;
    dup2(1,fd);
    while (1)
    {
        char buff[]="hello from p1";
        cout<<buff;
        fflush(stdout);
        sleep(10);
    }
}