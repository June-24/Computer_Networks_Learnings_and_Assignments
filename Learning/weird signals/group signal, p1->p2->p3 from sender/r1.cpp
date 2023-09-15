#include<bits/stdc++.h>
#include<unistd.h>
#include<signal.h>
// wait
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
void handle(int signo){
    cout<<"R1 received the message\n";
}
int main(){
    cout<<"R1 started\n";
    cout<<"Pid of R1: "<<getpid()<<endl;
    cout<<"Gid of R1: "<<getpgid(getpid())<<endl;
    int c=0;
    c=fork();
    if(c>0){
        signal(SIGUSR1,handle);
        sleep(3);
        while(1){
            sleep(3);
            cout<<"R1 is still waiting for signal"<<endl;
        } 
    }
    else{
        // r2 process
        char *path="./r2";
        char *argv[]={"./r2",NULL};
        execv(path,argv); 
    }
    

}
