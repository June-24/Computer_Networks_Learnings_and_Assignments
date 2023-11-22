#include "header.h"
using namespace std;
void error(string s)
{
	perror(s.c_str());
	exit(1);
}  

sem_t semaphore;
set<int> ports;
set<int> created_service;
struct thread_input
{
	int port;
	int sfd;
};

void printTCPHeader(u_char *packet, int ipheadersize)
{
	struct tcphdr *tcpHeader = (struct tcphdr *)(packet + ipheadersize);

	// cout << "TCP Header : ############################\n";
	cout << "    Source port : " << ntohs(tcpHeader->th_sport) << endl;
	cout << "    Destination port : " << ntohs(tcpHeader->th_dport) << endl;
	// 	cout << "    sequence : " << ntohl(tcpHeader->th_seq) << endl;
	// 	cout << "    acknowledge : " << ntohl(tcpHeader->th_ack) << endl;
	// 	cout << "    data offset : " << (tcpHeader->th_off << 2) << " Bytes" << endl;
	// 	cout << "    Flags : " << static_cast<int>(tcpHeader->th_flags) << endl;
	// 	cout << "    window size : " << ntohs(tcpHeader->th_win) << endl;
	// 	cout << "    chesksum : 0x" << hex << ntohs(tcpHeader->th_sum) << dec << endl;
	// 	cout << "    Urgent pointer : " << ntohs(tcpHeader->th_urp) << endl;
}

void *observe(void *arg)
{
	int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (rsfd < 0)
		error("rsfd");

	// struct sockaddr_in addr;
	// addr.sin_family = AF_INET;
	// addr.sin_addr.s_addr = INADDR_ANY;
	// addr.sin_port = 0; // Port is ignored in raw sockets
	// if (bind(rsfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	// 	error("bind");

	u_char packet[65535];
	int server_port;

	while (1)
	{
		ssize_t packet_size = recv(rsfd, packet, sizeof(packet), 0);
		if (packet_size < 0)
			error("recv");
		struct ip *ipHeader = (struct ip *)packet;

		// Check if the packet is coming from the loopback interface
		if (ipHeader->ip_src.s_addr != htonl(INADDR_LOOPBACK) || ipHeader->ip_dst.s_addr != htonl(INADDR_LOOPBACK))
			continue;
		struct tcphdr *tcpHeader = (struct tcphdr *)(packet + ipHeader->ip_hl * 4);
		server_port = ntohs(tcpHeader->th_dport);

		//@ skipping printing if no data present
		if (created_service.find(server_port) != created_service.end())
			continue;
		//@ skipping if sfd already created on that port
		if ((ipHeader->ip_hl * 4 + tcpHeader->th_off * 4 - packet_size) == 0)
			continue;

		printTCPHeader(packet, ipHeader->ip_hl * 4);
		cout << "Payload (interpreted as a string): ";
		for (unsigned int i = ipHeader->ip_hl * 4 + tcpHeader->th_off * 4; i < packet_size; ++i)
		{
			cout << packet[i];
		}
		cout << "\nPacket Captured " << endl;
		cout << "found new server port : " << server_port << endl;
		fflush(stdout);

		sem_wait(&semaphore);
		ports.insert(server_port);
		sem_post(&semaphore);
	}
}

void *fake_service(void *arg)
{
	struct thread_input *in = (struct thread_input *)arg;
	int sfd = in->sfd;
	int port = in->port;

	struct sockaddr_in clientAddr;
	clientAddr.sin_family = AF_INET;
	int size = sizeof(clientAddr);
	cout << "FAKE SERVER THREAD READY FOR " << port << "\n\n";

	while (1)
	{
		int nsfd = accept(sfd, (struct sockaddr *)&clientAddr, (socklen_t *)&size);
		if (nsfd < 0)
			error("accept");

		char buf[1024];
		int n = recv(nsfd, buf, sizeof(buf), 0);
		if (n < 0)
			error("recv");
		else if (n == 0)
			continue;
		buf[n] = '\0';
		cout << "FAKE SERVER RECEIVED : " << buf << endl;
		cout << "client successfully stolen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n";
		fflush(stdout);
	}
}
int main()
{
	if (sem_init(&semaphore, 0, 1) < 0)
		error("sem_init");

	pthread_t sniffer;
	if (pthread_create(&sniffer, NULL, observe, NULL) < 0)
		error("pthread_create");
	cout << "sniffer thread created\n";

	while (1)
	{
		for (int port : ports)
		{
			// skip if already have sfd open at this port
			if (created_service.find(port) != created_service.end())
			{
				usleep(100000);
				continue;
			}

			int sfd = socket(AF_INET, SOCK_STREAM, 0);
			if (sfd < 0)
				error("socket");

			created_service.insert(port);

			int optval = 1;
			if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) < 0)
				error("setsockopt");

			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			addr.sin_port = htons(port);

			if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
				perror("bind");

			if (listen(sfd, 5) < 0)
				error("listen");

			pthread_t fake_server;
			struct thread_input *arg = new struct thread_input;
			arg->port = port;
			arg->sfd = sfd;

			if (pthread_create(&fake_server, NULL, fake_service, arg) < 0)
				error("pthread_create");
		}
	}

	return 0;
}
