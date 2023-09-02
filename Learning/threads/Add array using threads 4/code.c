#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int arr[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
int sum[4]={0,0,0,0};
int part=0;
void* fun(void * arg){
    int tid=*(int*)arg;
    printf("Thread %d is printing this\n",tid);
    int tpart=part++;

    for(int i=tpart*(16/4);i<(tpart+1)*(16/4);i++){
        sum[tpart]+=arr[i];
    }
    
}

int main(){
    pthread_t tid[4];
    // create 4 threads and sum the array
    for(int i=0;i<4;i++){
        pthread_create(&tid[i],NULL,fun,(void*)&tid[i]);
    }
    for(int i=0;i<4;i++){
        pthread_join(tid[i],NULL);
    }
    int total=0;
    for(int i=0;i<4;i++){
        total+=sum[i];
    }
    printf("The sum is %d\n",total);
    // print the sum
    exit(0);


}