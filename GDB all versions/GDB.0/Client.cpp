#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;
int main(){
    // sending details of file
    mkfifo("NITWGDB",0666);
    int wfd=open("NITWGDB",O_WRONLY);
    char buff[]={'P','.','c','p','p'};
    write(wfd,buff,sizeof(buff));

    // getting details about the result
    mkfifo("RETURN1",0666);
    int rfd=open("RETURN1",O_RDONLY);
    char buff1[10];
    read(rfd,buff1,10);
    cout<<buff1<<endl;

}