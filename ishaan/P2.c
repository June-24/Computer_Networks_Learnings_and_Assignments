#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/poll.h>

int main()
{
    int ffd = open("myfifo", O_WRONLY);

    char msg[] = "This info has arrived from process 2";

    write(ffd, msg, sizeof(msg));

    close(ffd);

    return 0;
}