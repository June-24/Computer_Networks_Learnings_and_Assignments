#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
// TWO pipe between parent and child
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
        write(pp1[1],"dio",3);
        read(pp2[0],buff2,10);
        printf("parent read : %s\n            ",buff2);


    }
    else{
        //child process
        // closing the pp1 for the child(writing part)
        close(pp1[1]);
        close(pp2[0]);
        read(pp1[0],buff2,3);
        printf("child read: %s\n",buff2);
        write(pp2[1],"lmao",4);
    }


}
