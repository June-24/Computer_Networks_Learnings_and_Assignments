#include<iostream>
using namespace std;
int main(){
    string s;
    cout<<"this is the p2 process"<<endl;
    cout<<"reading from the input file using cin"<<endl<<endl;
    while(getline(cin,s)){
        cout<<s<<endl;
    }
}