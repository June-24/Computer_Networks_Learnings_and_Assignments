// #include<bits/stdc++.h>
// #include<signal.h>
// #include<unistd.h>
// #include<sys/wait.h>
// using namespace std;
// void PtoC(int sig){
//     cout<<"signal from parent to child"<<endl;
// }
// void CtoP(int signal){
//     cout<<"signal from child to parent"<<endl;
// }
// int main(){
//     signal(SIGUSR1,PtoC);
//     signal(SIGUSR2,CtoP);
//     int c=0;
//     c=fork();
//     if(c>0){
//         // parent
//         signal(SIGUSR2,CtoP);
//         kill(c,SIGUSR1);
//     }
//     else{
//         // child
//         signal(SIGUSR1,PtoC);
//         kill(getppid(),SIGUSR2);

//     }

// }



#include <bits/stdc++.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void PtoC(int sig) {
    cout << "Signal from parent to child" << endl;
}

void CtoP(int sig) {
    cout << "Signal from child to parent" << endl;
}

int main() {
    signal(SIGUSR1, PtoC);
    signal(SIGUSR2, CtoP);
    int c = 0;
    c = fork();
    if (c > 0) {
        // parent
        sleep(1); // Add a delay to allow child process to set up its signal handler
        kill(c, SIGUSR1);
        wait(NULL);
    } else {
        // child
        pause(); // Wait for the signal from the parent
        kill(getppid(), SIGUSR2);
    }
}

