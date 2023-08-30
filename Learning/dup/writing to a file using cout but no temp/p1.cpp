#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;
int main(){
    int ofd=open("out.txt",O_WRONLY);
    cout<<"yowai mo"<<endl;
    dup2(ofd,1);
    cout<<"writng to a file"<<endl;
    cout<<"lmao"<<endl;

}