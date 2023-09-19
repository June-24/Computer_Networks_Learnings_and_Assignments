#include <iostream>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <cstring>
#include <cstdlib> // for exit()
using namespace std;

struct buff {
    long type; // Change to long
    char bufff[100];
};

int main() {
    key_t k1 = ftok("./t1.txt", 65);
    key_t k2 = ftok("./t2.txt", 65);

    int msqid1 = msgget(k1, 0666 | IPC_CREAT);
    int msqid2 = msgget(k2, 0666 | IPC_CREAT);

    struct buff b1;
    struct buff b2;

    while (1) {
        cout << "Enter the message: ";
        b1.type = 1; // Change to 1 (or any other non-zero value)
        cin.getline(b1.bufff, sizeof(b1.bufff));
        msgsnd(msqid1, &b1, sizeof(struct buff) - sizeof(long), 0); // Subtract long size

        msgrcv(msqid2, &b2, sizeof(struct buff), 2, 0); // Subtract long size
        cout << "Message received: " << b2.bufff << endl;
    }

    return 0;
}
