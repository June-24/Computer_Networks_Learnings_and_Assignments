#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;
int main(){
    
    int ifd=open("in.txt",O_RDONLY);
    int ofd=open("in.txt",O_WRONLY);
    // parent will writeto a file, then 
    // child will exec p2 which will 
    // read from input using cin
    int c=fork();
    if(c>0){
        // parent process
        cout<<"parent process now"<<endl<<endl;
        dup2(ofd,1);
        cout<<"first line using cout"<<endl;
        cout<<"second line\n\n"<<endl;
        cout<<"last line"<<endl;

    }
    else{
        // child process
        cout<<"child process now"<<endl<<endl;
        dup2(ifd,0);
        char* pos[]={NULL};
        execvp("./p2",pos);
    }
}