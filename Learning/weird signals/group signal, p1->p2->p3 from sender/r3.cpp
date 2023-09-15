#include<bits/stdc++.h>
#include<unistd.h>
#include<signal.h>
// wait
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
void handle(int signo){
    cout<<"R3 received the message\n";
}
int main(){
    cout<<"R3 started\n";
    cout<<"Pid of R3: "<<getpid()<<endl;
    cout<<"Gid of R3: "<<getpgid(getpid())<<endl;
    int c=0;
    c=fork();
    if(c>0){
        signal(SIGUSR1,handle);
        sleep(3);
        while(1){
            sleep(3);
            cout<<"R3 is still waiting for signal"<<endl;
        }
    }else{
        // r3 process
        char *path="./r4";
        char *argv[]={"./r4",NULL};
        execv(path,argv);
    }
    
}