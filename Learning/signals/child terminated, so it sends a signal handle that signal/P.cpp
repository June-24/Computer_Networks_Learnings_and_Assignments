#include<bits/stdc++.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;
void handler(int signal){
    cout<<"Sigchld received"<<endl;
}
int main(){
    int c=0;
    c=fork();
    if(c>0){
        //parent
        signal(SIGCHLD,handler);
        sleep(1);
        pause();
    }
    else{
        //child
        cout<<"Child process"<<endl;
        exit(0);
    }
}