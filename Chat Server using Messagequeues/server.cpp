#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

struct my_buffer {
    int type;
    char data[100];
    int pid; // Server's PID
} message;

int main() {
    key_t key_server = ftok("./server_queue.txt", 65);
    int msqid_server = msgget(key_server, 0666 | IPC_CREAT);

    // Use an array of message queues for multiple clients
    int msqid_clients[10]; // Assuming a maximum of 10 clients

    for (int i = 0; i < 2; ++i) {
        key_t key_client = ftok("./client_queue.txt", i + 1);
        msqid_clients[i] = msgget(key_client, 0666 | IPC_CREAT);
    }

    while (1) {
        msgrcv(msqid_server, &message, sizeof(message), 0, 0);
        
        if (message.type == 1) {
            // Received a client message
            printf("Received from Client [%d]: %s\n", message.pid, message.data);

            // Broadcast received message to all connected clients
            for (int i = 0; i < 2; ++i) {
                if (msqid_clients[i] != -1) {
                    msgsnd(msqid_clients[i], &message, sizeof(message), 0);
                }
            }
        }
    }

    return 0;
}
