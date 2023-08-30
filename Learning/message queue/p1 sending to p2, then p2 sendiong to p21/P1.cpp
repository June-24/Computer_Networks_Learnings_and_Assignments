#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
using namespace std;


struct my_buffer {
    long type;
    char data[100];
} message;

int main() {
    key_t key = ftok("./t.txt", 65);
    int msqid = msgget(key, 0666 | IPC_CREAT);

    message.type = 1;

    printf("P1 - Write Data: ");
    fgets(message.data, sizeof(message.data), stdin);
    msgsnd(msqid, &message, sizeof(message.data), 0);

    msgrcv(msqid, &message, sizeof(message.data), 2, 0);
    cout<<message.type<<endl;
    printf("P1 - Received Response: %s\n", message.data);

    return 0;
}
