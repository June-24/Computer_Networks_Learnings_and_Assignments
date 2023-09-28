#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<sys/poll.h>
#include <unistd.h>

using namespace std;

int main (){

    //the four ports
    vector<int> ports = {50001,50002,50003,50004};

    //making the sfds
    vector<int> sfds(4);

    for(int i=0; i<4; i++){
        sfds[i] = socket(AF_INET , SOCK_STREAM , 0);
        if (sfds[i]==-1) perror("Socket error");

        int reuse = 1;
        if (setsockopt(sfds[i], SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

        if (setsockopt(sfds[i], SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
        perror("setsockopt(SO_REUSEPORT) failed");

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_port = htons(ports[i]);
        address.sin_addr.s_addr = INADDR_ANY;

        //Bind
        int a = bind (sfds[i] , (struct sockaddr *)&address , sizeof(address));
        if (a == -1) perror("Bind error");

        //Listen
        int b = listen(sfds[i] , 10);
        if (b == -1) perror("Listen error");
    }


    cout<<"Listening"<<endl;


    //poll
    struct pollfd pfds[4];
    for(int i=0; i<4; i++){
        struct pollfd temp;
        temp.fd = sfds[i];
        temp.events = POLLIN;
        pfds[i] = temp;
    }
    int curr=4;

    while(1){

        int n = poll(pfds,curr,100);

        if(n==-1) perror("poll");
        
        else if(n==0) continue;

        else{

            struct sockaddr * client_address;
            socklen_t len = sizeof(client_address);
            
            for(int i=0; i<4; i++){

                if(pfds[i].revents && POLLIN){
                    int nsfd = accept(pfds[i].fd , (struct sockaddr*)&client_address , &len);

                    int c = fork();

                    if(c==0){       //child process

                        sleep(1);

                        //we first attach our standard input to nsfd so that we
                        //don't lose it when we exec

                        //Also our service should already listen to that port 
                        dup2(nsfd,0);
                        
                        char *arr[] = {NULL};
                        if(i==0) execvp("./S1.out",arr);
                        if(i==1) execvp("./S2.out",arr);
                    }

                    else{           //parent process

                        //close the sfd for that service and also the nsfd made
                        //also change the fd in pfds so that poll no longer waits for that
                        close(pfds[i].fd);
                        close(nsfd);
                        pfds[i].fd = 0;

                        //Life goes on usual from here for the super server
                    }
                }
            }
        }

    }
}