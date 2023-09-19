#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
// for wait
#include <sys/wait.h> 


using namespace std;
int main(){
    int c=fork();
    if(c>0){
        // parent
        wait(NULL);
        cout<<"hello";
    }
    else{   
        // child
        int ifd=open("./input.txt",O_RDONLY);
        int ofd=open("./out.txt",O_WRONLY);
        dup2(ifd,0);
        dup2(ofd,1);
        // path for the p2 prog
        char* path[]={NULL};
        // execvp
        execvp("./p2",path);
    }
}
