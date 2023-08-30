#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
// one pipe between parent and child
using namespace std;
int main(){
    int pp[2];
    char buff[20];char buff2[20];
    pipe(pp);
    int c=fork();
    if(c>0){
        // parent process
        // closing the pp0 for parent(read part])
        close(pp[0]);
        write(pp[1],"dio",3);

    }
    else{
        //child process
        // closing the pp1 for the child(writing part)
        close(pp[1]);
        read(pp[0],buff2,3);
        printf("%s",buff2);
    }


}
