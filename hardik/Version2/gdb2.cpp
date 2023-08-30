#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <fstream>
#include <string.h>
#include <fcntl.h>

using namespace std;

int main()
{
    //Famous FIFO
    mkfifo("NITWGDB",0666);
    int gfd = open("NITWGDB",O_RDONLY);
    struct pollfd pfds[1];
    pfds[0].fd = gfd;
    pfds[0].events = POLLIN;

    while (1)
    {
        int ret = poll(pfds,1,10);
        if (ret > 0)
        {
            if (pfds[0].revents & POLLIN)
            {
                //Obtaining PID and opening Named Pipe for that user
                char userLogin[20];
                read(pfds[0].fd,userLogin,20);
                mkfifo(userLogin,0666);
                int ufd = open(userLogin,O_RDONLY);
                char buffer[500];
                read(ufd,buffer,500);
                close(ufd);

                //Creating temporary cpp program for user code
                ofstream m0file("temp.cpp",ofstream::out | ofstream::trunc);
                m0file.close();

                int tfd = open("temp.cpp",O_TRUNC|O_WRONLY);
                write(tfd,buffer,strlen(buffer));
                close(tfd);

                memset(buffer,0,500);


                //Compiling user code
                system("g++ temp.cpp -o temp");
                
                //Redirecting Standar I/O to in.txt and tout.txt
                int originalRead = dup(0), originalWrite = dup(1);
                int tofd = open("tout.txt",O_WRONLY);
                int infd = open("in.txt",O_RDONLY);
                dup2(infd,0);
                dup2(tofd,1);

                //Forking to execute user program
                int c = 0;
                c = fork();
                if (c>0)
                {
                    //Wait for User Program to finish execution
                    wait(NULL);
                    ofstream m1file("temp.cpp",ofstream::out | ofstream::trunc);
                    m1file.close();

                    //Reverting Standard I/O to original state
                    dup2(originalRead,0);        
                    dup2(originalWrite,1);
                    close(tofd);
                    close(infd);

                    //Comparing User Output and Exepcted Output
                    ifstream userOutput("tout.txt");
                    ifstream testOutput("out.txt");
                    string curUserLine, curTesterLine;
                    char finalMessage[50] = "Test Cases Passed";

                    while (!userOutput.eof() && !testOutput.eof())
                    {
                        getline(userOutput,curUserLine);
                        getline(testOutput,curTesterLine);
                        if (curUserLine != curTesterLine)
                        {
                            strcpy(finalMessage,"Test Cases Failed");
                            break;
                        }
                    }
                    userOutput.close();
                    testOutput.close();

                    //Sending Verdict to user through Named Pipe
                    int ufd = open(userLogin,O_WRONLY);
                    write(ufd,finalMessage,strlen(finalMessage));
                    close(ufd);
                }
                else
                {
                    //Execute User Program
                    char* args[] = {"./temp",NULL};
                    execvp(args[0],args);
                    cerr << "Failed to Execute\n" ;  
                }
            }
        }
    }

}