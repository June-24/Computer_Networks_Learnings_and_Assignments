#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>

using namespace std;
char IP_packet[8192], *payload; // packet and the payload

struct iphdr *IPhdr = (struct iphdr *)IP_packet;
struct tcphdr *TCPHdr = (struct tcphdr *)(IP_packet + sizeof(struct iphdr));
void initialiseIP()
{
    IPhdr->ihl = 5;
    IPhdr->version = 4;
    IPhdr->tos = 0;
    IPhdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(payload));
    IPhdr->id = htons(8888);
    IPhdr->frag_off = 0;
    IPhdr->ttl = 255;
    IPhdr->protocol = IPPROTO_TCP;// using TCP
    IPhdr->check = 0;
    IPhdr->daddr = inet_addr("127.0.0.1");   // destination IP
}
void initiliseTCP()
{
    TCPHdr->seq = 0;
    TCPHdr->ack_seq = 0;
    TCPHdr->doff = 0;
    TCPHdr->fin = 0;
    TCPHdr->syn = 1;
    TCPHdr->rst = 0;
    TCPHdr->psh = 0;
    TCPHdr->ack = 0;
    TCPHdr->urg = 0;
    TCPHdr->window = htons(5840);
    TCPHdr->check = 0;
    TCPHdr->urg_ptr = 0;
}
int main()
{
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (rsfd < 0)
    {
        cout << "Error in creating socket\n";
        exit(0);
    }
    int one = 1;
    if (setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)
    {
        perror("setsockopt");
        exit(0);
    }
    // server payload
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Allocate memory for the IP header
    payload = (char *)malloc(8192);
    memset(payload, '\0', 8192);
    payload = IP_packet + sizeof(struct iphdr) + sizeof(struct tcphdr);

    const char *data = "IP spoofing from sender";
    strcpy(payload, data);

    initialiseIP();
    initiliseTCP();

    IPhdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(data));
    while (1)
    {
        // changing sender IP randomly
        string IP=to_string(rand()%255)+"."+to_string(rand()%255)+"."+to_string(rand()%255)+"."+to_string(rand()%255);
        IPhdr->saddr = inet_addr(IP.c_str()); // source IP
        // sending the packet
        if (sendto(rsfd, IP_packet, ntohs(IPhdr->tot_len), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            perror("sendto error");
            exit(0);
        }
        cout << "Packet Sent thru IP : " << inet_ntoa(*(struct in_addr *)&IPhdr->saddr) << endl;
        sleep(2);
    }
}
