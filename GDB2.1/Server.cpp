#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;

const int MAX_CLIENTS = 3;

int main() {
    mkfifo("FamousFifo", 0666);
    int ffd = open("FamousFifo", O_RDONLY);
    
    struct pollfd pfds[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        pfds[i].fd = -1; // Initialize with invalid value
    }
    pfds[0].fd = ffd;
    pfds[0].events = POLLIN;

    while (1) {
        int ret = poll(pfds, MAX_CLIENTS, 10);
        
        if (ret > 0) {
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (pfds[i].revents & POLLIN) {
                    char pid[10];
                    read(pfds[i].fd, pid, sizeof(pid));
                    
                    mkfifo(pid, 0666);
                    int ifd = open(pid, O_RDONLY);
                    char input[5000];
                    ssize_t bytesRead = read(ifd, input, sizeof(input) - 1);
                    close(ifd);

                    input[bytesRead] = '\0'; // Null-terminate the input

                    ofstream f1("T.cpp", ofstream::out | ofstream::trunc);
                    f1.close();

                    int tfd = open("T.cpp", O_WRONLY);
                    write(tfd, input, bytesRead);
                    close(tfd);

                    system("g++ T.cpp -o T");

                    int c = fork();
                    if (c > 0) {
                        wait(NULL);
                        // Rest of the code...
                        ofstream file("T.cpp", ofstream::out | ofstream::trunc);
                        file.close();
                        ifstream outfd("out.txt");
                        ifstream poutfd("Pout.txt");
                        string s1,s2;
                        bool flag=true;
                        while(getline(outfd,s1) && getline(poutfd,s2)){
                            if(s1!=s2){
                                flag=false;
                                break;
                            }
                        }
                        if(flag){
                            char res[]="Passed Test case";
                            int ofd=open(pid,O_WRONLY);
                            write(ofd,res,strlen(res));
                            close(ofd);
                        }
                        else{
                            char res[]="Failed Test case";
                            int ofd=open(pid,O_WRONLY);
                            write(ofd,res,strlen(res));
                            close(ofd);
                        }
                    } else if (c == 0) {
                        int iifd=open("in.txt",O_RDONLY);
                        int oofd=open("Pout.txt",O_WRONLY|O_TRUNC);
                        dup2(iifd,0);
                        dup2(oofd,1);
                        char *path[] = {NULL};
                        execvp("./T",path);
                    }
                }
            }
        }
    }

    return 0;
}

