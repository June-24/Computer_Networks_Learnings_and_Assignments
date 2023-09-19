

#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char* filename = "example.txt";
    int fd;

    // Open the file in append mode (O_APPEND flag)
    fd = open(filename, O_WRONLY | O_APPEND);

    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Now you can write data to the file, and it will be appended to the existing content
    const char* data = "This is some new data to append.";
    write(fd, data, strlen(data));

    // Don't forget to close the file descriptor when done
    close(fd);

    return 0;
}
