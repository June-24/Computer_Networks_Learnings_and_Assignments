#include "hdr.h"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	int pid = 0, n;
	char buf[1024];
	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(1619);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

	pid = fork();
	while (1)
	{
		if (pid == 0)
		{
			while ((n = read(0, buf, 1024)) > 0)
			{
				buf[n - 1] = '\0';
				write(sockfd, buf, n);
			}
		}
		else
		{
			while ((n = read(sockfd, buf, 1024)) > 0)
			{
				buf[n] = '\0';
				printf("Message: %s\n", buf);
			}
		}
	}
	exit(0);
}
