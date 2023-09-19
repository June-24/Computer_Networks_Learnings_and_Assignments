#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct my_buffer {
    long type; // Change int to long for message type
    char data[100];
} message; 

int main() {
    // Create a key
    key_t key = ftok("./t.txt", 65);

    // Create a message queue and return identifier
    int msqid = msgget(key, 0666 | IPC_CREAT);

    message.type = 1; // Set the message type

    printf("Write Data: ");
    fgets(message.data, sizeof(message.data), stdin);

    // Send the message
    int t = msgsnd(msqid, &message, sizeof(message.data), 0);
    if (t == -1) {
        perror("msgsnd");
    }

    printf("Data sent: %s\n", message.data);

    int r=msgrcv(msqid,&message,sizeof(message.data),2,0);
    if(r==-1)
    {
        perror("msgrcv");
    }
    printf("Data received from p2` %s\n", message.data);

    return 0;
}



























































