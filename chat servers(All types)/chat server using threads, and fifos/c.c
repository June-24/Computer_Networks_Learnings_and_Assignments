#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define FIFO_PATH "chat_fifo"

void *receiveMessages(void *arg) {
    int fd = open(FIFO_PATH, O_RDONLY);
    char buffer[BUFFER_SIZE];

    while (1) {
        ssize_t bytes_received = read(fd, buffer, sizeof(buffer));
        if (bytes_received <= 0) {
            perror("Server disconnected");
            exit(1);
        }
        buffer[bytes_received] = '\0';
        printf("%s", buffer);
    }

    pthread_exit(NULL);
}

int main() {
    int client_fd = open(FIFO_PATH, O_WRONLY);

    if (client_fd == -1) {
        perror("Server is not running");
        exit(1);
    }

    char name[20];
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strlen(name) - 1] = '\0';

    write(client_fd, name, sizeof(name));

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receiveMessages, NULL);

    char message[BUFFER_SIZE];
    while (1) {
        fgets(message, sizeof(message), stdin);
        write(client_fd, message, strlen(message) + 1); // Include the null terminator
    }

    return 0;
}
