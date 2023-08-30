#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main() {
    string s;
    cin>>s;
    cout<<s<<endl;
    cout << "hello from p2" << endl;
    
    dup2( STDOUT_FILENO,1);
    cout << "hello from p2 again" << endl;

    return 0;
}
