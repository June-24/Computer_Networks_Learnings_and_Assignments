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
    key_t key = ftok("./t.txt", 65);
    int msqid = msgget(key, 0666 | IPC_CREAT);

    while (1) {
        msgrcv(msqid, &message, sizeof(message), 0, 0);
        printf("Received from Client [%d]: %s\n", message.type, message.data);
    }

    return 0;
}
