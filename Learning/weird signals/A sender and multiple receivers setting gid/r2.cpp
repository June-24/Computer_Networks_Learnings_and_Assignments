#include <iostream>
#include <unistd.h>
#include <signal.h>
using namespace std;
void handle(int sig) {
    cout << "Signal received in receiver 2" << std::endl;
}
int main() {
    cout<<"pgid using getpgid:"<<getpgid(getpid())<<endl;
    system("sudo usermod -g 6969 junaid");
    cout<<"pgid using getpgid:"<<getpgid(getpid())<<endl;

    signal(SIGUSR1, handle);
    pause();

    return 0;
}
