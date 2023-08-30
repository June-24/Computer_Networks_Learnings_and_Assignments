#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;
int main(){
    int tfd=0;
    int ofd=open("out.txt",O_WRONLY);
    cout<<"yowai mo"<<endl;
    dup2(1,tfd);
    dup2(ofd,1);
    cout<<"writng to a file"<<endl;
    cout<<"lmao"<<endl;
    dup2(tfd,1);
    cout<<"yo after writing to a file"<<endl;

}