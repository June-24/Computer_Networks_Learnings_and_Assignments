#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main() {
    // for all three different services
    struct sockaddr_in ServerAddr[3];
    int sfd[3];
    for (int i = 0; i < 3; i++) {
        sfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        ServerAddr[i].sin_family = AF_INET;
        ServerAddr[i].sin_addr.s_addr = INADDR_ANY;
        ServerAddr[i].sin_port = htons(8899 + i); 
        // bind and listen
        int ret = bind(sfd[i], (struct sockaddr *)&ServerAddr[i], sizeof(ServerAddr[i]));
        ret = listen(sfd[i], 10);
    }

    cout << "Server open at ports 8899, 8900, 8901" << endl;
    struct sockaddr_in ClientAddr;
    socklen_t len = sizeof(ClientAddr);

    
    while (1) {
        // polling
        struct pollfd pfds[3];
        int nsfd[3]={-1,-1,-1};

        // Initialize pollfd for listening sockets
        for (int i = 0; i < 3; i++) {
            pfds[i].fd = sfd[i];
            pfds[i].events = POLLIN;
        }

        // Wait indefinitely for an event
        int ret = poll(pfds, 3, -1); 
        if (ret == -1) {
            perror("poll error");
            exit(1);
        }

        int j=-1;
        // Check if there's a new connection on any of the sockets
        for (int i = 0; i < 3; i++) {
            if (pfds[i].revents & POLLIN) {
                nsfd[i] = accept(sfd[i], (struct sockaddr *)&ClientAddr, &len);
                if (nsfd[i] == -1) {
                    perror("accept error");
                }
                j=i;
                break; // break when any client arrives
            }
        }

        if (nsfd[j] != -1) {
            // Create a child process to handle the client
            int c = fork();
            if (c > 0) {
                // Parent process
                close(nsfd[j]);
            } 
            else if (c == 0) {
                // Child process
                for (int i = 0; i < 3; i++) {
                    close(sfd[i]); 
                }
                dup2(nsfd[j], 0); // Redirect stdin to the socket
                char buffer[1024];
                ssize_t bytesRead;
                while ((bytesRead = recv(nsfd[j], buffer, sizeof(buffer) - 1, 0)) > 0) {
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

                if (bytesRead == -1) {
                    perror("recv error");
                }
                exit(0);
            }
        }
    }

    return 0;
}



