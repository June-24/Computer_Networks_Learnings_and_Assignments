#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


void * fun(void * arg){
    // sleep(2);
    printf("Thread is printing this\n");
    return NULL;
}

int main(){
    pthread_t tid;
    printf("before creating a thread\n");
    pthread_create(&tid,NULL,fun,NULL);
    printf("after creating a thread\n");
    pthread_join(tid,NULL);
    printf("after joining a thread\n");
    exit(0);
}