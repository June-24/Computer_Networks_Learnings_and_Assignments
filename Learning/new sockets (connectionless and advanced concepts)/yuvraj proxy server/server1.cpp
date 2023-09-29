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
#define SPORT1 3040
int sfd;
int main()
{
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(SPORT1);
	int opt = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		perror("opt");
	}
	int e = bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
	if (e < 0)
	{
		perror("binding error");
	}
	listen(sfd, 3);
	int k = sizeof(addr);
	while (true)
	{
		int nsfd = accept(sfd, (struct sockaddr *)&addr, (socklen_t *)&k);
		char buffer[1024];
		recv(nsfd, buffer, sizeof(buffer), 0);
		cout << buffer << endl;
	}
	return 0;
}
