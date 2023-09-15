#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// threads
#include <pthread.h>
using namespace std;

struct sockaddr_in ServerAddr[3];
int sfd[3];

void *client_handle(void *arg) {
    int i=*(int *)arg;
    cout << "Server open at port: "<< 8899 + i << endl;
    struct sockaddr_in ClientAddr;
    socklen_t len = sizeof(ClientAddr);
    int nsfd = accept(sfd[i], (struct sockaddr *)&ClientAddr, &len);
    if (nsfd == -1) {
        perror("accept error");
    }
    int c=0;
    c=fork();
    if(c>0){
        // parent 
        close(nsfd);
    }
    else{
        // child
        close(sfd[i]);
        dup2(nsfd, 0); // Redirect stdin to the socket
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = recv(nsfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytesRead] = '\0';
            cout << "Client: " << buffer << endl;
            if (strcmp(buffer, "s1") == 0) {
                char *args[] = {"./s1", NULL};
                execvp(args[0], args);
            } 
            else if (strcmp(buffer, "s2") == 0) {
                char *args[] = {"./s2", NULL};
                execvp(args[0], args);
            } 
            else if (strcmp(buffer, "s3") == 0) {
                char *args[] = {"./s3", NULL};
                execvp(args[0], args);
            }
        }
        close(nsfd);
        exit(0);
    }
}


int main() {
    pthread_t tid[3];
    // create three threads for three different services
    for (int i = 0; i < 3; i++) {
        sfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        ServerAddr[i].sin_family = AF_INET;
        ServerAddr[i].sin_addr.s_addr = INADDR_ANY;
        ServerAddr[i].sin_port = htons(8899 + i); 
        // bind and listen
        int ret = bind(sfd[i], (struct sockaddr *)&ServerAddr[i], sizeof(ServerAddr[i]));
        ret = listen(sfd[i], 10);
    }
    cout<<"before thread";
    // int j=0;
    int args[3];
    for(int i=0;i<3;i++){
        args[i]=i;
        pthread_create(&tid[i], NULL, client_handle, (void *)&args[i]);
    }

    for(int i=0;i<3;i++)
        pthread_join(tid[i],NULL);
    

    return 0;
}



