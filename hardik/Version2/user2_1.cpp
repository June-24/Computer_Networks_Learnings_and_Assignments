#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

using namespace std;

int main()
{
    int p = getpid();
    string temp = to_string(p) + 'A';
    const char* myPID = temp.c_str();

    //Sending PID to Server through Famous FIFO
    mkfifo("NITWGDB",0666);
    int gfd = open("NITWGDB",O_WRONLY);
    write(gfd,myPID,strlen(myPID));
    close(gfd);

    //Reading User Code from file
    char buffer[500];
    int pfd = open("pgm1.cpp",O_RDONLY);
    read(pfd,buffer,500);
    close(pfd);

    //Creating a pipe for client and sending code
    mkfifo(myPID,0666);
    int cfd = open(myPID,O_WRONLY);
    write(cfd,buffer,strlen(buffer));
    close(cfd);

    //Receiving Verdict from Server using Named Pipe
    char verdict[50];
    int ufd = open(myPID,O_RDONLY);
    read(ufd,verdict,50);
    close(ufd);
    cout << "Verdict for " << myPID << " - " << verdict << endl;
}