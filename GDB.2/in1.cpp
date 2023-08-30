#include <bits/stdc++.h>
#include <sys/poll.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

using namespace std;

int main() {
    mkfifo("IN1", 0666);
    int wfd = open("IN1", O_WRONLY);
    char buff[20];
    int rfd1 = open("P1.cpp", O_RDONLY);
    
    // reading the cpp file and sending the code to server 
    int bytesRead;
    while ((bytesRead = read(rfd1, buff, sizeof(buff) - 1)) > 0) {
        buff[bytesRead] = '\0'; 
        write(wfd, buff, bytesRead);
    }
    close(wfd);
    mkfifo("OUT1", 0666);
    int rfd=open("OUT1",O_RDONLY);
    char buff1[20];

    bytesRead = read(rfd, buff1, sizeof(buff1) - 1);
    buff1[bytesRead] = '\0';
    cout<<"The test case has "<<buff1<<endl;
    close(rfd);

    

}
