#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
using namespace std;
int main(){

    // It and Pout file opening
    int itfd=open("IT.txt",O_RDONLY);
    int poutfd=open("Pout.txt",O_WRONLY);

    // mkfifo
    // making two fifo, because some confusion happening
    mkfifo("NITWGDB",0666);
    mkfifo("RETURN1",0666);

    // receiving the file info
    int rfd=open("NITWGDB",O_RDONLY);
    char buff[5];
    read(rfd,buff,5);

    // executing the file
    const char arr[18]={'g','+','+',' ',buff[0],buff[1],buff[2],buff[3],buff[4],' ','-','o',' ','P'};
    system(arr);
    
    // fork
    int c=fork();
    if(c>0){
        // parent
        cout<<"Inside parent now"<<endl;
        cout<<"Parent going into Wait state"<<endl;
        wait(NULL);
        cout<<"Parent out of wait state"<<endl;

        // after the file has been exec
        cout<<"Program has been executed"<<endl;

        // need to check if Pout and Out are same or not
        ifstream outfd("./OT.txt");
        ifstream poutfd("Pout.txt");
        int flag=0;
        string s1,s2;
        while(getline(outfd,s1)&&getline(poutfd,s2)){
            if(s1!=s2){
                flag=1;
            }
        }
        
        int wfd=open("RETURN1",O_WRONLY);

        if(flag==1){
            // wrong
            write(wfd,"Wrong",5);
        }
        else{
            //correct
            write(wfd,"Correct",7);
        }

        
    }
    else{
        // child
        cout<<"inside child Process now"<<endl;
        
        // so that the P process can directly read from IT.txt and write to Pout.txt
        dup2(poutfd,1);
        dup2(itfd,0);
        char* path[]={NULL};
        // calling exec
        execvp("./P",path);

    }
}