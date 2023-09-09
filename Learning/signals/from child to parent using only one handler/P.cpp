#include<bits/stdc++.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;
void handler(int signal){
    if(signal==SIGUSR1){
        cout<<"from parent to child"<<endl;
    }
    else{
        cout<<"from child to parent"<<endl;
    }
}
int main(){
    int c=0;
    c=fork();
    if(c>0){
        //parent
        signal(SIGUSR1,handler);
        sleep(1);
        kill(c,SIGUSR2);
        pause();
    }
    else{
        // child
        signal(SIGUSR2,handler);
        pause();
        sleep(1);
        kill(getppid(),SIGUSR1);
    }
}