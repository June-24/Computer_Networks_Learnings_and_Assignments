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
int rsfd;
struct sockaddr_in ServerAddr, ClientAddr;
void print_hdr(char *buff)
{
    struct iphdr *IPhdr = (struct iphdr *)buff;

    struct sockaddr_in source, dest;
    memset(&source, 0, sizeof(source));
    memset(&dest, 0, sizeof(dest));

    source.sin_addr.s_addr = IPhdr->saddr;
    dest.sin_addr.s_addr = IPhdr->daddr;

    cout << "IP Header ->: " << endl;
    cout << "\tSource IP       : " << inet_ntoa(source.sin_addr) << endl;
    cout << "\tDestination IP  : " << inet_ntoa(dest.sin_addr) << endl;
    cout << "\tVersion         : " << (unsigned int)IPhdr->version << endl;
    cout << "\tHeader length(bytes): " << ((unsigned int)(IPhdr->ihl)) * 4 << endl;
    cout << "\tType of service : " << (unsigned int)IPhdr->tos << endl;
    cout << "\tTotal length    : " << IPhdr->tot_len << endl;
    cout << "\tIdentification  : " << ntohs(IPhdr->id) << endl;
    cout << "\tFragment offset : " << IPhdr->frag_off << endl;
    cout << "\tTime to live    : " << (unsigned int)IPhdr->ttl << endl;
    cout << "\tProtocol        : " << (unsigned int)IPhdr->protocol << " (TCP)" << endl;
    cout << "\tChecksum        : " << ntohs(IPhdr->check) << endl;

    // Now, let's print the TCP header
    struct tcphdr *TCPHdr = (struct tcphdr *)(buff + (IPhdr->ihl) * 4);
    cout << "TCP Header ->: " << endl;
    cout << "\tSource Port     : " << ntohs(TCPHdr->source) << endl;
    cout << "\tDestination Port: " << ntohs(TCPHdr->dest) << endl;
    cout << "\tSequence Number : " << ntohl(TCPHdr->seq) << endl;
    cout << "\tAcknowledgment Number: " << ntohl(TCPHdr->ack_seq) << endl;
    cout << "\tData Offset     : " << TCPHdr->doff << " (Header length in 32-bit words)" << endl;
    cout << "\tFlags           : " << endl;
    cout << "\t\tURG: " << TCPHdr->urg << endl;
    cout << "\t\tACK: " << TCPHdr->ack << endl;
    cout << "\t\tPSH: " << TCPHdr->psh << endl;
    cout << "\t\tRST: " << TCPHdr->rst << endl;
    cout << "\t\tSYN: " << TCPHdr->syn << endl;
    cout << "\t\tFIN: " << TCPHdr->fin << endl;
    cout << "\tWindow Size     : " << ntohs(TCPHdr->window) << endl;
    cout << "\tChecksum        : " << ntohs(TCPHdr->check) << endl;
    cout << "\tUrgent Pointer  : " << TCPHdr->urg_ptr << endl;
}
// void print_hdr(char *buff)
// {
//     struct iphdr *IPhdr = (struct iphdr *)buff;

//     struct sockaddr_in source, dest;
//     memset(&source, 0, sizeof(source));
//     memset(&dest, 0, sizeof(dest));

//     source.sin_addr.s_addr = IPhdr->saddr;
//     dest.sin_addr.s_addr = IPhdr->daddr;

//     cout << "IP Header ->: " << endl;
//     cout << "\tSource IP       : " << inet_ntoa(source.sin_addr) << endl;
//     cout << "\tDestination IP  : " << inet_ntoa(dest.sin_addr) << endl;
//     cout << "\tVersion         : " << (unsigned int)IPhdr->version << endl;
//     cout << "\tHeader length(bytes): " << ((unsigned int)(IPhdr->ihl)) * 4 << endl;
//     cout << "\tType of service : " << (unsigned int)IPhdr->tos << endl;
//     cout << "\tTotal length    : " << IPhdr->tot_len << endl;
//     cout << "\tIdentification  : " << ntohs(IPhdr->id) << endl;
//     cout << "\tFragment offset : " << IPhdr->frag_off << endl;
//     cout << "\tTime to live    : " << (unsigned int)IPhdr->ttl << endl;
//     cout << "\tProtocol        : " << (unsigned int)IPhdr->protocol << endl;
//     cout << "\tChecksum        : " << ntohs(IPhdr->check) << endl;
// }

void createSocket()
{
    rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (rsfd < 0)
    {
        cout << "Error in creating socket\n";
        exit(0);
    }
    int one = 1;
    if (setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)
    {
        cout << "Error in setsockopt\n";
        exit(0);
    }
    // server needs to bind its own address
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(rsfd, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
    {
        cout << "Error in binding\n";
        exit(0);
    }
}
int main()
{
    createSocket();
    int count = 0;
    while (1)
    {
        // buff is the entire packet
        char buff[1000];
        socklen_t len = sizeof(ServerAddr);
        if (recvfrom(rsfd, buff, 100, 0, (struct sockaddr *)&ServerAddr, &len) < 0)
        {
            cout << "Error in recvfrom\n";
            exit(0);
        }
        struct iphdr *IPhdr = (struct iphdr *)buff;

        // buff1 has the IP address
        char buff1[100];
        struct in_addr addr;
        addr.s_addr = IPhdr->saddr;
        strcpy(buff1, inet_ntoa(addr));

        cout << "Source IP: " << buff1 << endl;
        const char *payload = buff + sizeof(struct iphdr) + sizeof(struct tcphdr);
        cout << "data: " << payload << endl;
        
        if (count++ == 0)
        {
            // printing the header only once
            print_hdr(buff);
        }
    }
}