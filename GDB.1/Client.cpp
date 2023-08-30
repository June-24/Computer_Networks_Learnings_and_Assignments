#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main() {
    //opening the fifo NITWGDB for sending the P.cpp code to server
    mkfifo("NITWGDB", 0666);


    int wfd = open("NITWGDB", O_WRONLY);
    char buff[20];
    int rfd1 = open("P.cpp", O_RDONLY);
    
    // reading the cpp file and sending the code to server 
    ssize_t bytesRead;
    while ((bytesRead = read(rfd1, buff, sizeof(buff) - 1)) > 0) {
        buff[bytesRead] = '\0'; 
        write(wfd, buff, bytesRead);
    }

    close(wfd); 

    // Getting details about the result
    // making another fifo for receiving the output
    mkfifo("RETURN1", 0666);
    int rfd = open("RETURN1", O_RDONLY);
    char buff1[10];

    // printing if correct or not
    bytesRead=read(rfd, buff1, sizeof(buff1) - 1);
    buff1[bytesRead] = '\0'; 
    cout <<"The output is : "<< buff1 << endl;

    close(rfd);
}
