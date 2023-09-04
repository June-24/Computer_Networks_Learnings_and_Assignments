#include<iostream>
#include<pthread.h>
#include<sys/wait.h>
using namespace std;
void* read(void* args){
    while(1){
        cout<<"Enter something : "<<endl;
        sleep(2);
    }
}
void* write(void* args){
    while(1){
        string s;
        cin>>s;
        cout<<"the string is : "<<s<<endl;
    }
}
int main(){
    pthread_t t1,t2;
    pthread_create(&t1,NULL,read,NULL);
    pthread_create(&t2,NULL,write,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
}
