#include <iostream>
#include <cstring>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct Message {
    long messageType;
    char messageText[100];
};

key_t readKey, writeKey;
int readQueueId, writeQueueId;
int totalClients = 0;
long clientPids[100];

void registerClient(long pid) {
    for (int i = 0; i < totalClients; i++) {
        if (clientPids[i] == pid) {
            return;
        }
    }

    clientPids[totalClients++] = pid;
    Message message;
    message.messageType = pid + 1;
    strcpy(message.messageText, "101");
    msgsnd(readQueueId, &message, sizeof(message), 0);
}

void cleanup(int sig) {
    msgctl(readQueueId, IPC_RMID, NULL);
    msgctl(writeQueueId, IPC_RMID, NULL);
    exit(0);
}

int main() {
    readKey = ftok("server", 102);
    writeKey = ftok("client", 101);
    readQueueId = msgget(readKey, 0666 | IPC_CREAT);
    writeQueueId = msgget(writeKey, 0666 | IPC_CREAT);

    while (true) {
        Message message;
        message.messageType = 0;

        if (msgrcv(readQueueId, &message, sizeof(message), message.messageType, 0) != -1) {
            registerClient(message.messageType);

            for (int i = 0; i < totalClients; i++) {
                if (clientPids[i] == message.messageType) {
                    continue;
                }
                cout << "Receiving from: " << clientPids[i] << " " << message.messageText << endl;

                Message messageToSend;
                messageToSend.messageType = clientPids[i];
                strcpy(messageToSend.messageText, message.messageText);
                msgsnd(writeQueueId, &messageToSend, sizeof(messageToSend), 0);
            }
            signal(SIGINT, cleanup);
        }
    }

    return 0;
}
