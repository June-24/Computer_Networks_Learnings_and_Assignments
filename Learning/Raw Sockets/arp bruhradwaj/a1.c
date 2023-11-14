#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>

int main() {
    int sockfd;
    struct ether_arp arp_packet;
    struct sockaddr socket_address;

    // Create a raw socket that listens for ARP packets
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Loop to receive ARP packets
    while (1) {
        socklen_t socket_address_length = sizeof(socket_address);
        ssize_t received_bytes = recvfrom(sockfd, &arp_packet, sizeof(arp_packet), 0, &socket_address, &socket_address_length);

        if (received_bytes == -1) {
            perror("recvfrom");
            exit(1);
        }

        // Check if it's an ARP packet (Ethernet frame type 0x0806)
        if (ntohs(arp_packet.arp_hrd) == ARPHRD_ETHER && ntohs(arp_packet.arp_pro) == ETH_P_IP) {
            printf("Received ARP packet:\n");
            printf("Sender MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", arp_packet.arp_sha[0], arp_packet.arp_sha[1], arp_packet.arp_sha[2], arp_packet.arp_sha[3], arp_packet.arp_sha[4], arp_packet.arp_sha[5]);
            printf("Sender IP Address: %d.%d.%d.%d\n", arp_packet.arp_spa[0], arp_packet.arp_spa[1], arp_packet.arp_spa[2], arp_packet.arp_spa[3]);
            printf("Target MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", arp_packet.arp_tha[0], arp_packet.arp_tha[1], arp_packet.arp_tha[2], arp_packet.arp_tha[3], arp_packet.arp_tha[4], arp_packet.arp_tha[5]);
            printf("Target IP Address: %d.%d.%d.%d\n", arp_packet.arp_tpa[0], arp_packet.arp_tpa[1], arp_packet.arp_tpa[2], arp_packet.arp_tpa[3]);
            printf("\n");
        } else {
            printf("other packet received\n");
        }
    }

    close(sockfd);

    return 0;
}