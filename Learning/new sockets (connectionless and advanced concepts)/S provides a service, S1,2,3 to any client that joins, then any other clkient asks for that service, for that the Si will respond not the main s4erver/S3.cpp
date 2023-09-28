#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
using namespace std;

int main()
{
    int sfd = 10;
    dup2(1, sfd);
    // way of getting back STDOUT lmao
    int tty_fd = open("/dev/tty", O_WRONLY);
    dup2(tty_fd, 1);
    cout<<endl;
    cout << "S3 service is exec by S" << endl<<endl;

    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    char buffer[1024];
    while (1)
    {
        int n = recvfrom(sfd, buffer, 1024, 0, (struct sockaddr *)&client, &len);
        getpeername(sfd, (struct sockaddr *)&client, &len);

        if (n > 0)
        {
            if (n > 0)
            {
                buffer[n] = '\0';
                cout<<endl;
                cout << "client port: " << ntohs(client.sin_port) << "   Client IP: " << inet_ntoa(client.sin_addr) << endl; // ntohs is used to convert network byte order to host byte order
                cout << "service S1(from exec): " << buffer << endl
                     << endl;
            }
        }
    }
}