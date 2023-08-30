#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

struct my_buffer
{
    int type;
    char data[100];
} message;

int main()
{
    key_t key = ftok("./t.txt", 65);
    int msqid = msgget(key, 0666 | IPC_CREAT);

    message.type = getpid();
    while (1)
    {
        printf("Client [%d] - Write Data: ", message.type);
        fgets(message.data, sizeof(message.data), stdin);
        msgsnd(msqid, &message, sizeof(message), 0);
    }

    return 0;
}
