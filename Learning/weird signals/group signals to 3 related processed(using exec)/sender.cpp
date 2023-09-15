#include<bits/stdc++.h>
#include<unistd.h>
#include<signal.h>
using namespace std;

int main(){
    int gid;
    cout<<"Enter the GID number: ";
    cin>>gid;
    killpg(gid, SIGUSR1);
}