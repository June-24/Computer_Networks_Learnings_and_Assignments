#include<bits/stdc++.h>
#include<unistd.h>
#include<signal.h>
// wait
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
void handle(int signo){
    cout<<"R2 received the message\n";
}
int main(){
    cout<<"R2 started\n";
    cout<<"Pid of R2: "<<getpid()<<endl;
    cout<<"Gid of R2: "<<getpgid(getpid())<<endl;
    int c=0;
    c=fork();
    if(c>0){
        signal(SIGUSR1,handle);
        sleep(3);
        while(1){
            sleep(3);
            cout<<"R2 is still waiting for signal"<<endl;
        }
    }else{
        // r3 process
        char *path="./r3";
        char *argv[]={"./r3",NULL};
        execv(path,argv);
    }
    
}