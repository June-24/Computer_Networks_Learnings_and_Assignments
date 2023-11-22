#include "header.h"
// ! MUST DO HTONS IF WE WANT SAME PORT NO TO BE SHOWN IN SNIFFER
using namespace std;
void error(string s)
{
	perror(s.c_str());
	exit(1);
}

vector<int> sfd(3);
map<int, set<int>> clients;
sem_t semaphore;
//! IMPORTANT TO USE SEMAPHORE TO AVOID RACE CONDITION
//* else thread will try to accept and insert into map at the same tile main while loop
//* erases elements from map => map is not updated properly
//* => what should be deleted from map is not deleted
//* => we are calling recv on closed nsfd => bad fd error

void *accept_clients(void *arg)
{
	int *in = (int *)arg;
	int sfd = *in;
	cout << "Thread for " << sfd << " created\n";
	while (1)
	{
		struct sockaddr_in clientaddr;
		int size = sizeof(clientaddr);
		int nsfd = accept(sfd, (struct sockaddr *)&clientaddr, (socklen_t *)&size);
		if (nsfd < 0)
		{
			error("accept");
		}
		// lock sem before accessing clients
		sem_wait(&semaphore);
		clients[sfd].insert(nsfd);
		sem_post(&semaphore);
	}
}

int main()
{
	if (sem_init(&semaphore, 0, 1))
		error("sem_init");

	for (int i = 0; i < 3; ++i)
	{
		int a = socket(AF_INET, SOCK_STREAM, 0);
		sfd[i] = a;
		if (sfd[i] < 0)
			error("socket");
	}

	struct sockaddr_in addr[3];
	addr[0].sin_family = AF_INET;
	addr[1].sin_family = AF_INET;
	addr[2].sin_family = AF_INET;
	addr[0].sin_addr.s_addr = inet_addr("127.0.0.1");
	addr[1].sin_addr.s_addr = inet_addr("127.0.0.1");
	addr[2].sin_addr.s_addr = inet_addr("127.0.0.1");

	// ! MUST DO HTONS IF WE WANT SAME PORT NO TO BE SHOWN IN SNIFFER
	addr[0].sin_port = htons(1111);
	addr[1].sin_port = htons(2222);
	addr[2].sin_port = htons(3333);

	pthread_t threads[3];
	for (int i = 0; i < 3; ++i)
	{
		int *optval = new int;
		*optval = 1;
		if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) < 0)
			error("setsockopt");

		if (bind(sfd[i], (struct sockaddr *)&addr[i], sizeof(addr[i])) < 0)
			error("bind");

		if (listen(sfd[i], 5) < 0)
			error("listen");

		int *arg = new int;
		*arg = sfd[i];
		if (pthread_create(&threads[i], NULL, accept_clients, arg) < 0)
			error("pthread_create");
	}

	while (1)
	{
		for (auto &[key, set] : clients)
		{
			if (set.empty())
				continue;

			vector<int> finished;
			for (int nsfd : set)
			{
				char buf[1024];
				int n = recv(nsfd, buf, sizeof(buf), 0);
				if (n < 0)
					error("recv");
				if (n == 0)
					break;
				buf[n] = '\0';
				cout << "received : " << buf << endl;
				finished.push_back(nsfd);
			}

			// get lock before editing set
			for (int nsfd : finished)
			{
				sem_wait(&semaphore);
				set.erase(nsfd);
				sem_post(&semaphore);

				close(nsfd);
				cout << "erased " << nsfd << endl;
			}
		}
	}

	sem_destroy(&semaphore);
}