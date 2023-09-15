#include<bits/stdc++.h>
#include<unistd.h>
// wait
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
int main(){
    cout<<"Service s1 started->>>>>>>>>>>>>>>>>>>: "<<endl;
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = recv(0, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytesRead] = '\0';
        if(strcmp(buffer,"exitt")==0){
            break;
        }
        cout << "service 1 Received: " << buffer << endl;
    }
    if (bytesRead == -1)
    {
        perror("recv error");
    }
    cout<<"Service s1 Ended->>>>>>>>>>>>>>>>>>>: "<<endl;
}