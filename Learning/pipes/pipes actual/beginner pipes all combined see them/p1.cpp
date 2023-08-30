#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
// for wait
#include <sys/wait.h> 

using namespace std;
int main(){
    int fd1=open("in.txt",O_RDONLY);
    int fd2=open("out.txt",O_WRONLY);

    char * buff[10];
    while(read(fd1,buff,10)){
        write(fd2,buff,10);
    }
}
