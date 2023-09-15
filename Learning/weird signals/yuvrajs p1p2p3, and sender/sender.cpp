#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
using namespace std;

int main(){
    int p1,p2,p3;
    cout<<"Enter the pids of p1 p2 and p3 to restart them from halt"<<endl;
    cin>>p1>>p2>>p3;
    string t="kill -CONT "+to_string(p1) ;
    system(t.c_str()) ;
    t="kill -CONT "+to_string(p2) ;
    system(t.c_str()) ;
    t="kill -CONT "+to_string(p3) ;
    system(t.c_str()) ;

	int gpid ;
	printf("Enter GPID Number to send signal : ") ;
	scanf("%d", &gpid) ;
	killpg(gpid, SIGINT) ;
}
