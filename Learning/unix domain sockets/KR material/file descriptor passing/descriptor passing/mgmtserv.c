#include "hdr.h"
ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd)
{
	struct msghdr msg;
	struct iovec iov[1];

	union
	{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;
	struct cmsghdr *cmptr;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);

	cmptr = CMSG_FIRSTHDR(&msg);
	cmptr->cmsg_len = CMSG_LEN(sizeof(int));
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	*((int *)CMSG_DATA(cmptr)) = sendfd;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	return (sendmsg(fd, &msg, 0));
}

int main(int argc, char **argv)
{
	int sockfd, listenfd, connfd;
	char buff[MAXLINE];
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	struct sockaddr_un saddr;
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);

	bzero(&saddr, sizeof(saddr));
	saddr.sun_family = AF_LOCAL;
	strcpy(saddr.sun_path, UNIXSTR_PATH);
	if (connect(sockfd, (SA *)&saddr, sizeof(saddr)) == 0)
		printf("Connection with Servicing server successfull\n");

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	while (1)
	{
		clilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0)
		{
			if (errno == EINTR)
				continue;
			else
				err_sys("accept error");
		}
		if (getpeername(connfd, (SA *)&cliaddr, &clilen) == 0)
		{
			printf("connection from %s, port %d\n",
				   inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
				   ntohs(cliaddr.sin_port));
		}

		write_fd(sockfd, "", 1, connfd);

		close(connfd);
	}
}
