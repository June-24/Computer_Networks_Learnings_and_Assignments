#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

int main() {
    int rsfd;
    struct sockaddr_in source_addr;
    socklen_t addr_len = sizeof(source_addr);

    // Create a raw socket to listen for ICMP packets
    rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (rsfd < 0) {
        perror("socket");
        exit(1);
    }

    while (1) {
        char buffer[1024];
        ssize_t bytes_received = recvfrom(rsfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&source_addr, &addr_len);
        if (bytes_received < 0) {
            perror("recvfrom");
            break;
        }

        // Parse and process the ICMP packet (buffer)
        struct icmphdr *icmp = (struct icmphdr *)(buffer);

        // Check if it's an ICMP echo request (ICMP type 8)
        if (icmp->type == ICMP_ECHO) {
            // Craft an ICMP echo reply (ICMP type 0) and send it back
            icmp->type = ICMP_ECHOREPLY;
            icmp->checksum = 0; // Recalculate the checksum if necessary
            sendto(rsfd, buffer, bytes_received, 0, (struct sockaddr*)&source_addr, addr_len);
            printf("yoyooy\n");
        }
    }

    close(rsfd);
    return 0;
}
