#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

struct my_buffer {
    int type;
    char data[100];
    int pid; // Client's PID
} message;

int main() {
    key_t key_server = ftok("./server_queue.txt", 65);
    int msqid_server = msgget(key_server, 0666 | IPC_CREAT);

    key_t key_client = ftok("./client_queue.txt", 2);
    int msqid_client = msgget(key_client, 0666 | IPC_CREAT);

    while (1) {
        msgrcv(msqid_client, &message, sizeof(message), 0, 0);
        printf("Received from Server: %s\n", message.data);
        printf("Client [%d] - Write Data: ", getpid());
        fgets(message.data, sizeof(message.data), stdin);
        message.type = 1; // Indicate it's a client message
        message.pid = getpid();
        
        // Send message to server
        msgsnd(msqid_server, &message, sizeof(message), 0);

        // Receive response from server
        msgrcv(msqid_client, &message, sizeof(message), 0, 0);
        printf("Received from Server: %s\n", message.data);
    }

    return 0;
}
