#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/udp.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <iostream>
using namespace std;

struct myarphdr
{
    uint16_t htype;  // Hardware type
    uint16_t ptype;  // Protocol type
    uint8_t hlen;    // Hardware address length
    uint8_t plen;    // Protocol address length
    uint16_t opcode; // Operation code
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
};

void print_arp_header(const struct myarphdr *arp_header)
{
    cout << "ARP Header:" << endl;
    cout << "Hardware Type: " << ntohs(arp_header->htype) << endl;
    cout << "Protocol Type: " << ntohs(arp_header->ptype) << endl;
    cout << "Hardware Address Length: " << (int)arp_header->hlen << endl;
    cout << "Protocol Address Length: " << (int)arp_header->plen << endl;
    cout << "Operation Code: " << ntohs(arp_header->opcode) << endl;

    cout << "Sender MAC Address: ";
    for (int i = 0; i < 6; i++)
    {
        cout << hex << (int)arp_header->sender_mac[i];
        if (i < 5)
            cout << ":";
    }
    cout << dec << endl;

    cout << "Sender IP Address: ";
    for (int i = 0; i < 4; i++)
    {
        cout << dec << (int)arp_header->sender_ip[i];
        if (i < 3)
            cout << ".";
    }
    cout << endl;

    // You can similarly print the target MAC and IP addresses
}

int main()
{
    int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (packet_socket == -1)
    {
        perror("Failed to create packet socket");
        return 1;
    }

    char buffer[2048]; // Adjust the buffer size as needed

    while (true)
    {
        ssize_t bytes_received = recv(packet_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1)
        {
            perror("Failed to receive packet");
            break;
        }

        // Print Ethernet header
        struct ether_header *eth_header = (struct ether_header *)buffer;

        // Calculate the size of the Ethernet header
        size_t eth_header_size = sizeof(struct ether_header);

        // Check if the packet is an ARP packet (Ethernet Type 0x0806)
        if (ntohs(eth_header->ether_type) == ETH_P_ARP && bytes_received >= eth_header_size + sizeof(struct myarphdr))
        {
            // Print ARP header
            struct myarphdr *arp_header = (struct myarphdr *)(buffer + eth_header_size);
            print_arp_header(arp_header);
        }
    }

    close(packet_socket);

    return 0;
}