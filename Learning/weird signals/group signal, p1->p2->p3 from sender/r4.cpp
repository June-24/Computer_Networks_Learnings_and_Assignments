#include<bits/stdc++.h>
#include<unistd.h>
#include<signal.h>
// wait
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;
void handle(int signo){
    cout<<"R4 received the message\n";
}
int main(){
    cout<<"R4 started\n";
    cout<<"Pid of R4: "<<getpid()<<endl;
    cout<<"Gid of R4: "<<getpgid(getpid())<<endl;
    signal(SIGUSR1,handle);
    sleep(3);
    while(1){
        sleep(3);
        cout<<"R4 is still waiting for signal"<<endl;
    }
}