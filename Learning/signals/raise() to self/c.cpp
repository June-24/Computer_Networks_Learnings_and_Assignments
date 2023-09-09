#include<bits/stdc++.h>
#include<signal.h>
using namespace std;
void handler(int sig){
    cout << "Signal " << sig << " received" << endl;
    
}
int main(){
    // as we can see SIGKILL cannot he handled lmao
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);
    signal(SIGKILL, handler);
    // raise(SIGKILL);
    raise(SIGINT);
    raise(SIGUSR1);
    raise(SIGKILL);

    

}