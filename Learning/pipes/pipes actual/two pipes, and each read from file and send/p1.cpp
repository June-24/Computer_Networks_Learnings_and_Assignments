#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
// TWO pipe between parent and child and files reading and sending
using namespace std;
int main(){
    int pp1[2];
    int pp2[2];
    char buff[20];char buff2[20];
    pipe(pp1);
    pipe(pp2);
    int c=fork();
    if(c>0){
        // parent process
        // closing the pp0 for parent(read part])
        close(pp1[0]);
        close(pp2[1]);
        char buff3[100];
        // reading fro minput1 and sending to child
        int fd1=open("input1.txt",O_RDONLY);
        while(read(fd1,buff3,100))
            write(pp1[1],buff3,100);
        read(pp2[0],buff2,100);
        printf("parent read : %s\n",buff2);
    }
    else{
        //child process
        // closing the pp1 for the child(writing part)
        close(pp1[1]);
        close(pp2[0]);
        char buff3[100];
        read(pp1[0],buff2,100);
        printf("child read: %s\n",buff2);
        int fd1=open("input2.txt",O_RDONLY);
        while(read(fd1,buff3,100))
            write(pp2[1],buff3,100);
    }
}
