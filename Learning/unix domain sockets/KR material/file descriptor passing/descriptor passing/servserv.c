#include "hdr.h"
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd)
{
	struct msghdr msg;
	struct iovec iov[1];
	ssize_t n;
	union
	{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;
	struct cmsghdr *cmptr;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if ((n = recvmsg(fd, &msg, 0)) <= 0)
		return (n);
	if ((cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
		cmptr->cmsg_len == CMSG_LEN(sizeof(int)))
	{
		if (cmptr->cmsg_level != SOL_SOCKET)
			err_quit("control level != SOL_SOCKET");
		if (cmptr->cmsg_type != SCM_RIGHTS)
			err_quit("control type != SCM_RIGHTS");
		*recvfd = *((int *)CMSG_DATA(cmptr));
	}
	else
		*recvfd = -1; /* descriptor was not passed */
	return (n);
}
int main(int argc, char **argv)
{
	int listenfd, connfd, sockfd = -1, n, pid = 0;
	char buff[MAXLINE];
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_un cliaddr, servaddr;
	struct sockaddr_in ca;
	listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);

	unlink(UNIXSTR_PATH);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

	bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	clilen = sizeof(cliaddr);
	if ((connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0)
	{
		if (errno == EINTR)
			exit(0);
		else
			err_sys("accept error");
	}

	while (read_fd(connfd, buff, 1, &sockfd) != -1)
	{
		printf("sockfd: %d\n", sockfd);
		if (getpeername(sockfd, (SA *)&ca, &clilen) == 0)
		{
			printf("new client: %s, port %d\n",
				   inet_ntop(AF_INET, &ca.sin_addr, buff, clilen), ntohs(ca.sin_port));
		}
		pid = fork();
		if (pid > 0)
			close(sockfd);
		else
		{
			while ((n = read(sockfd, buff, MAXLINE)) > 0)
			{
				buff[n] = '\0';
				printf("Message: %s\n", buff);
				write(sockfd, buff, n);
				if (!strcmp(buff, "exit"))
					break;
			}
			exit(0);
		}
	}
}
