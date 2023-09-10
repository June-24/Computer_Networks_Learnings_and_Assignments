#include<bits/stdc++.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
int noOfChildren=5;
int c=1;
using namespace std;
void handler(int signal){
    if(c==noOfChildren){
        cout<<"All "<<noOfChildren<<" children terminated"<<endl; 
    }
    else{
        c++;
    }
}
int main(){
    for(int i=0;i<noOfChildren;i++){
        int pid=fork();
        if(pid==0){
            sleep(2);
            cout<<"Child "<<i+1<<" with pid "<<getpid()<<" created"<<endl;
            cout<<"Terminating now"<<endl;
            exit(0);
        }
        else{
            signal(SIGCHLD,handler);
            pause();
        }
    }
    
}
