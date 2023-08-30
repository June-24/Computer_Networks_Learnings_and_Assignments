#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

using namespace std;
int main(){
    // need to read from file and write to file using cin and ciout
    int ifd=open("in.txt",O_RDONLY);
    int ofd=open("out.txt",O_WRONLY);
    int t1fd=0,t2fd=0;
    dup2(0,t1fd);
    dup2(1,t2fd);
    dup2(ifd,0);
    dup2(ofd,1);
    string s;
    while(getline(cin,s)){
        cout<<s<<endl;
    }
    dup2(t1fd,0);
    dup2(t2fd,1);
    cin>>s;
    cout<<s;
}