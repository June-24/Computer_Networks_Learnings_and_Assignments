#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sched.h>
#include <sys/wait.h>
using namespace std;

unordered_set<int> st;

struct temp {
    unordered_set<int>* st;
    int* fd;
};

int receive(void* args) {
    struct temp* sm = (struct temp*)args;
    int nsfd = *(sm->fd);
    unordered_set<int>& st = *(sm->st);
    while (1) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int n = recv(nsfd, buffer, sizeof(buffer), 0);
        if (n > 0) {
            for (auto i = st.begin(); i != st.end(); i++) {
                if (*i != nsfd) send(*i, buffer, sizeof(buffer), 0);
            }
        }
    }
}

int main() {
    void* memory = malloc(1024 * 1024);
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(1351);
    add.sin_addr.s_addr = inet_addr("127.0.0.1");
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    bind(sfd, (struct sockaddr*)&add, sizeof(add));
    listen(sfd, 100);

    while (1) {
        int nsfd = accept(sfd, NULL, NULL);
        st.insert(nsfd);
        struct temp* args = new struct temp;
        args->st = &st;
        args->fd = &nsfd;
        clone(receive, memory + 1024 * 1024, SIGCHLD, args);
    }
    close(sfd);
}
