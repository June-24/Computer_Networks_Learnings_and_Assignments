#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
// for wait
#include <sys/wait.h> 

using namespace std;
int main(){
    int pp[2];
    pipe(pp);
    char buffer[]={'a','b','c','d','d','e'};
    char buffer2[10];
    write(pp[1],buffer,6);
    read(pp[0],buffer2,6);
    printf("%s",buffer2);


}
