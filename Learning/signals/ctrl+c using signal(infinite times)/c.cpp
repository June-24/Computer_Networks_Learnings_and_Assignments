#include<bits/stdc++.h>
#include<signal.h>
using namespace std;
void handler(int signal){
    cout<<"ctrl+c pressed\n";
}
int main(){

    signal(SIGINT,handler);
    while(1);
    return 0;
}