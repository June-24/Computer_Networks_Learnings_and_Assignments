#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/poll.h>
#include<pthread.h>
#include <unistd.h>

using namespace std;

int main(){

    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1) perror("Socket error");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(60002);
    address.sin_addr.s_addr = INADDR_ANY;

    //Bind
    int a = bind (sfd , (struct sockaddr *)&address , sizeof(address));
    if (a == -1) perror("Bind error S1");

    //Listen
    int b = listen(sfd , 10);
    if (b == -1) perror("Listen error");

    if(a == -1 || b == -1) return 0;

    cout<<" -- Special Server 1 up and running --"<<endl;

    struct pollfd pfds[10];
    pfds[0].fd = sfd;
    pfds[0].events = POLLIN;
    int curr = 1;

    while(1){
        
        int n = poll(pfds, curr, 1000);


        if(n>0){
            for(int i=0; i<curr; i++){
                if(pfds[i].revents && POLLIN){

                    if(i == 0){
                        struct sockaddr * client_address;
                        socklen_t len = sizeof(client_address);

                        int nsfd = accept(sfd, (struct sockaddr*)&client_address , &len);

                        cout<<"New connection established "<<endl;

                        pfds[curr].fd = nsfd;
                        pfds[curr].events = POLLIN;
                        curr++;
                    }

                    else{
                        char buffer[100];
                        int n = recv(pfds[i].fd, buffer, 100, 0);

                        if(n>0){
                            buffer[n]='\0';
                            cout<<buffer<<endl;
                        }

                        string s = "Nice joke lol";
                        const char* buff = s.c_str();
                        send(pfds[i].fd,buff,strlen(buff)+1, 0);
                    }
                }
            }
        }
    }

    return 0;
}