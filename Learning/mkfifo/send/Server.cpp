#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
using namespace std;
int main(){
    // mkfifo
    // making two fifo, because some confusion happening
    mkfifo("NITWGDB",0666);
    mkfifo("RETURN1",0666);

    // receiving the file info
    int rfd=open("NITWGDB",O_RDONLY);
    char buff[5];
    read(rfd,buff,5);
    cout<<buff<<endl;
    
    int wfd=open("RETURN1",O_WRONLY);

    write(wfd,"Wrong",5);
}
