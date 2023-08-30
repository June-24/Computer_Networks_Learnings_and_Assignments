#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

struct my_buffer {
    int type;
    char data[100];
} message;

int main() {
    key_t key_server = ftok("./server_queue.txt", 65);
    int msqid_server = msgget(key_server, 0666 | IPC_CREAT);

    key_t key_client = ftok("./client_queue.txt", getpid());
    int msqid_client = msgget(key_client, 0666 | IPC_CREAT);

    while (1) {
        printf("Client [%d] - Write Data: ", getpid());
        fgets(message.data, sizeof(message.data), stdin);

        // Send message to server
        msgsnd(msqid_server, &message, sizeof(message), 0);

        // Receive response from server
        msgrcv(msqid_client, &message, sizeof(message), 0, 0);
        printf("Received from Server: %s\n", message.data);
    }

    return 0;
}
