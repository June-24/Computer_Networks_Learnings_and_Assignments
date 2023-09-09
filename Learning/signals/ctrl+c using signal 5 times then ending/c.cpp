#include<bits/stdc++.h>
#include<signal.h>
using namespace std;
int c=0;
void handler(int signal){
    if(c<5){
        cout<<"Ctrl+c pressed"<<endl;
        c++;
    }
    else{
        exit(0);
    }
}
int main(){
    signal(SIGINT,handler);
    while(1);
}