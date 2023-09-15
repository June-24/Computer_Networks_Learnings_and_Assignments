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
    int c1=0,c2=0;
    if((c1=fork())==0){
        // r2 process
        char *path="./r2";
        char *argv[]={"./r2",NULL};
        execv(path,argv); 
    }
    else{
        if((c2=fork())==0){
            // r3 process
            char *path="./r3";
            char *argv[]={"./r3",NULL};
            execv(path,argv);
        }
        else{
            signal(SIGUSR1,handle);
            sleep(3);
            while(1){
                sleep(3);
                cout<<"R1 is still waiting for signal"<<endl;
            }
        }
    }
    

}
