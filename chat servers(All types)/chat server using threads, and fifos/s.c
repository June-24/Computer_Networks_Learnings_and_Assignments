#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define FIFO_PATH "chat_fifo"

struct Client {
    char name[20];
};

struct Client clients[MAX_CLIENTS];
int client_count = 0;

void broadcastMessage(const char *message, int current_client) {
    for (int i = 0; i < client_count; i++) {
        if (i != current_client) {
            char fifo_name[20];
            sprintf(fifo_name, "%s_fifo", clients[i].name);

            int fd = open(fifo_name, O_WRONLY);
            if (fd != -1) {
                write(fd, message, strlen(message) + 1); // Include the null terminator
                close(fd);
            }
        }
    }
}

void *handleClient(void *arg) {
    int current_client = *(int *)arg;
    char fifo_name[20];
    sprintf(fifo_name, "%s_fifo", clients[current_client].name);

    int fd = open(fifo_name, O_RDONLY);

    char buffer[1024];
    char message[1024];

    while (1) {
        ssize_t bytes_received = read(fd, buffer, sizeof(buffer));
        if (bytes_received <= 0) {
            // Client disconnected
            unlink(fifo_name);
            printf("%s has left the chat.\n", clients[current_client].name);
            break;
        } else {
            buffer[bytes_received] = '\0';
            sprintf(message, "%s: %s", clients[current_client].name, buffer);
            broadcastMessage(message, current_client);
        }
    }

    pthread_exit(NULL);
}

int main() {
    mkfifo(FIFO_PATH, 0666);
    int server_fd = open(FIFO_PATH, O_RDWR);

    printf("Server started...\n");

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd != -1) {
            char client_name[20];
            read(client_fd, client_name, sizeof(client_name));

            if (client_count < MAX_CLIENTS) {
                strcpy(clients[client_count].name, client_name);

                printf("%s has joined the chat.\n", clients[client_count].name);

                pthread_t thread;
                pthread_create(&thread, NULL, handleClient, &client_count);
                client_count++;
            } else {
                printf("Chat room is full. %s cannot join.\n", client_name);
            }
        }
    }

    return 0;
}
