#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
    int pp[2];
    pipe(pp);

    int c = fork();

    if (c == 0) // Child process
    {
        close(pp[0]);
        char m1[] = "This message is from child";
        write(pp[1], m1, sizeof(m1));
        close(pp[1]);
        exit(0);
    }
    else // Parent process
    {
        close(pp[1]);
    }

    mkfifo("myfifo", 0666);
    int ff = open("myfifo", O_RDONLY);

    struct pollfd pfds[4];


    pfds[1].fd = pp[0];
    pfds[1].events = POLLIN;

    pfds[2].fd = ff;
    pfds[2].events = POLLIN;

    pfds[3].fd = 0; // stdin
    pfds[3].events = POLLIN;
    while (1)
    {
        

        int ret = poll(pfds, 4, 20);

        if (ret > 0)
        {
            for (int i = 1; i < 4; i++)
            {
                if (pfds[i].revents & POLLIN)
                {
                    char buffer[256];
                    int bytesRead = read(pfds[i].fd, buffer, sizeof(buffer));
                    if (bytesRead > 0)
                    {
                        buffer[bytesRead] = '\0';
                        printf("Read from fd %d: %s\n", pfds[i].fd, buffer);
                    }
                }
            }
        }
    }

    close(pp[0]);
    close(ff);
    return 0;
}
