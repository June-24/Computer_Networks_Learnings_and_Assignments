#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/wait.h>
using namespace std;
#define SIZE 100
int psfd;
struct sockaddr_in paddr;
int nsfd[SIZE];
int sc = 0, nc = 0;
struct pollfd npfd[SIZE];
#define PPORT 3030
#define SPORT1 3040
#define SPORT2 3050
pthread_t th;
void *fun(void *args)
{
	while (true)
	{
		int x = poll(npfd, nc, 10);
		if (x > 0)
		{
			for (int i = 0; i < nc; i++)
			{
				if (npfd[i].revents & POLLIN)
				{
					cout << "starting to execute a client" << endl;
					char buffer[1024];
					int n = read(nsfd[i], buffer, sizeof(buffer));

					cout << buffer << endl;
					int sfd;
					if (buffer[0] == '1')
					{
						sfd = socket(AF_INET, SOCK_STREAM, 0);
						sockaddr_in serverAddr;
						serverAddr.sin_family = AF_INET;
						serverAddr.sin_port = htons(SPORT1);
						serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
						if (connect(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
						{
							std::cerr << "Failed to connect to the server." << std::endl;
							close(psfd);
						}
						char message[1024];
						read(nsfd[i], message, sizeof(message));
						send(sfd, message, sizeof(buffer), 0);
						cout << "sent for service 1" << endl;
						close(nsfd[i]);
					}
					else if (buffer[0] == '2')
					{
						sfd = socket(AF_INET, SOCK_STREAM, 0);
						sockaddr_in serverAddr;
						serverAddr.sin_family = AF_INET;
						serverAddr.sin_port = htons(SPORT2);
						serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
						if (connect(sfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
						{
							std::cerr << "Failed to connect to the server." << std::endl;
							close(psfd);
						}
						char message[1024];
						read(nsfd[i], message, sizeof(message));
						send(sfd, message, sizeof(buffer), 0);
						cout << "sent for service 2" << endl;
						close(nsfd[i]);
					}
					close(sfd);
				}
			}
		}
	}
}
int main()
{
	psfd = socket(AF_INET, SOCK_STREAM, 0);
	paddr.sin_family = AF_INET;
	paddr.sin_addr.s_addr = INADDR_ANY;
	paddr.sin_port = htons(PPORT);
	int opt = 1;
	setsockopt(psfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	int e = bind(psfd, (struct sockaddr *)&paddr, sizeof(paddr));
	if (e < 0)
	{
		perror("binding error");
	}
	listen(psfd, 3);
	pthread_create(&th, NULL, &fun, NULL);
	while (true)
	{
		int k = sizeof(paddr);
		nsfd[nc] = accept(psfd, (struct sockaddr *)&paddr, (socklen_t *)&k);
		npfd[nc].fd = nsfd[nc];
		npfd[nc].events = POLLIN;
		nc++;
	}
}
