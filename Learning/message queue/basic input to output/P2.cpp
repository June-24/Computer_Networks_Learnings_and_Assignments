#include<iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
using namespace std;
struct my_buffer {
    long type; // Change int to long for message type
    char data[100];
} message;

int main() {
    // Create a key
    key_t key = ftok("./t.txt", 65);

    // Create a message queue and return identifier
    int msqid = msgget(key, 0666 | IPC_CREAT);

    // Receive the message with type 1
    msgrcv(msqid, &message, sizeof(message.data), 1, 0);

    printf("Data Received: %s\n", message.data);
    cout<<"enter a message to p1: ";
    fgets(message.data,sizeof(message.data),stdin);
    message.type=2;
    msgsnd(msqid,&message,sizeof(message.data),0);
    // Remove the message queue
    msgctl(msqid, IPC_RMID, NULL);

    return 0;
}
