#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>

int main(){
    int c=0;
    c=fork();
    if(c>0){
        // parent process
        wait(NULL);
        printf("this is parent process\n\n");
    }
    else{
        // child process
        printf("this is the child part just about to start\n\n");
        printf("f2 is going to execute p2\n\n");
        /*
        By adding fflush(stdout) after the print statements,
        you're explicitly flushing the output buffer,
        which will ensure that the output is immediately 
         displayed on the screen.
        */
        fflush(stdout); 
        char* path[]={"./p2",NULL};
        execvp(path[0],path);

    }
    printf("this is from parent, for child this gets overwrittn\n\n");
}