#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct my_buffer {
    long type;
    char data[100];
} message;

int main() {
    key_t key = ftok("./t.txt", 65);
    int msqid = msgget(key, 0666 | IPC_CREAT);
    while(1){
    msgrcv(msqid, &message, sizeof(message.data), 1, 0);
    printf("P2 - Received Message: %s\n", message.data);

    printf("P2 - Write Response: ");
    fgets(message.data, sizeof(message.data), stdin);
    message.type = 2; // Use type 2 for P2's responses
    msgsnd(msqid, &message, sizeof(message.data), 0);
    }
    return 0;
}
