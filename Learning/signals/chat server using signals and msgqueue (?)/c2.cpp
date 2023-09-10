#include <iostream>
#include <cstring>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

using namespace std;

struct Message {
    long messageType;
    char messageText[100];
};

key_t writeKey, readKey;
int writeQueueId, readQueueId;
int pid, projId;

void handleSigint(int sig) {
    cout << "C1(" << pid << "): ";
    Message message;
    cin.getline(message.messageText, sizeof(message.messageText));
    message.messageType = pid;
    msgsnd(writeQueueId, &message, sizeof(message), 0);
}

void firstTime() {
    cout << "Requesting to join group" << endl;
    Message message, receivedMessage;
    cin.getline(message.messageText, sizeof(message.messageText));
    message.messageType = static_cast<long>(pid);

    int result = msgsnd(writeQueueId, &message, sizeof(message), 0);
    if (result == -1) {
        perror("msgsnd");
        exit(1);
    }

    long tempType = static_cast<long>(pid + 1);
    if (msgrcv(writeQueueId, &receivedMessage, sizeof(receivedMessage), tempType, 0) != -1) {
        projId = atoi(receivedMessage.messageText);
    }
}

int main() {
    writeKey = ftok("server", 102);
    writeQueueId = msgget(writeKey, 0666 | IPC_CREAT);
    pid = getpid();

    firstTime();

    readKey = ftok("client", projId);
    readQueueId = msgget(readKey, 0666 | IPC_CREAT);

    while (true) {
        Message receivedMessage;
        signal(SIGINT, handleSigint);

        if (msgrcv(readQueueId, &receivedMessage, sizeof(receivedMessage), pid, 0) != -1) {
            cout << "received " << receivedMessage.messageText << endl;
        }
    }

    return 0;
}
