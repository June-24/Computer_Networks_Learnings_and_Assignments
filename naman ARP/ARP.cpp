#include <bits/stdc++.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <net/if.h>

using namespace std;

int main() 
{
    int rawSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));

    if (rawSocket == -1) 
    {
        perror("socket");
        return 1;
    }

    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);
    unsigned char buffer[1024];

    while (true) 
    {
        ssize_t dataSize = recvfrom(rawSocket, buffer, sizeof(buffer), 0, &saddr, &saddr_size);

        if (dataSize == -1) 
        {
            perror("recvfrom");
            close(rawSocket);
            return 1;
        }

        struct ethhdr *eth = (struct ethhdr *)(buffer);

        if (ntohs(eth->h_proto) == ETH_P_ARP) 
        {
            std::cout << "Received ARP packet" << std::endl;

            struct arphdr *arp = (struct arphdr *)(buffer + sizeof(struct ethhdr));

            if (ntohs(arp->ar_op) == ARPOP_REQUEST) 
            {
                std::cout << "ARP Request: ";
                unsigned char *ptr = buffer + sizeof(struct ethhdr) + sizeof(struct arphdr);
                struct in_addr source_ip;
                memcpy(&source_ip, ptr, sizeof(struct in_addr));

                ptr += sizeof(struct in_addr);
                unsigned char *mac_ptr = ptr;

                string destmac;
                std::cout << "Source IP: " << inet_ntoa(source_ip) << ", Source MAC: ";
                for (int i = 0; i < ETH_ALEN; i++) {
                    printf("%02X", mac_ptr[i]); // Print the MAC address byte in hexadecimal format
                    char byte[3];
                    snprintf(byte, sizeof(byte), "%02X", mac_ptr[i]); // Convert the byte to a two-character hexadecimal string
                    destmac += byte;
                    if (i != ETH_ALEN - 1) {
                        std::cout << ":";
                        destmac += ":";
                    }
                }
                std::cout << std::endl;


                struct in_addr dest_ip;
                memcpy(&dest_ip, ptr + sizeof(struct in_addr), sizeof(struct in_addr));

                // Extract Destination MAC
                unsigned char *dest_mac_ptr = ptr + sizeof(struct in_addr) * 2; // 2 times sizeof(struct in_addr)
                std::cout << "Destination IP: " << inet_ntoa(dest_ip) << ", Destination MAC: ";
                for (int i = 0; i < ETH_ALEN; i++) {
                    printf("%02X", dest_mac_ptr[i]);
                    if (i != ETH_ALEN - 1) {
                        std::cout << ":";
                    }
                }
                std::cout << std::endl;



                // Construct and send an ARP reply
                // Create an ARP reply packet
                unsigned char arp_reply[60];  // ARP reply packet size
                struct ethhdr *eth_reply = (struct ethhdr *)(arp_reply);
                struct arphdr *arp_reply_ptr = (struct arphdr *)(arp_reply + sizeof(struct ethhdr));
                
                // Fill in the Ethernet header
                // Set the source MAC address to your MAC address
                ether_aton_r("40:1C:83:A8:F1:79", (struct ether_addr *)eth_reply->h_source);
                // Set the destination MAC address to the source MAC from the request
                const char * dest_mac = destmac.c_str();
                ether_aton_r(dest_mac, (struct ether_addr *)eth_reply->h_dest);
                // Set the Ethernet type to ARP
                eth_reply->h_proto = htons(ETH_P_ARP);
                
                // Fill in the ARP header for the reply
                arp_reply_ptr->ar_hrd = htons(ARPHRD_ETHER);
                arp_reply_ptr->ar_pro = htons(ETH_P_IP);
                arp_reply_ptr->ar_hln = ETH_ALEN;  // MAC address length
                arp_reply_ptr->ar_pln = 4;         // IPv4 address length
                arp_reply_ptr->ar_op = htons(ARPOP_REPLY);  // ARP reply operation
                
                // Set the source MAC address to your MAC address
                ether_aton_r("40:1C:83:A8:F1:79", (struct ether_addr *)arp_reply_ptr + sizeof(struct ethhdr) + 6);
                // Set the source IP address to your IP address
                inet_pton(AF_INET, inet_ntoa(dest_ip) , arp_reply + sizeof(struct ethhdr) + 6 + ETH_ALEN);
                // Set the destination MAC address to the source MAC from the request
                ether_aton_r(dest_mac, (struct ether_addr *)arp_reply_ptr + sizeof(struct ethhdr) + 6 + ETH_ALEN + 4);
                // Set the destination IP address to the source IP from the request
                inet_pton(AF_INET, inet_ntoa(source_ip) , arp_reply + sizeof(struct ethhdr) + 6 + ETH_ALEN + 4 + ETH_ALEN);
                
                // Send the ARP reply
                ssize_t bytes_sent = send(rawSocket, arp_reply, sizeof(arp_reply), 0);
                if (bytes_sent == -1) {
                    perror("send");
                } else {
                    std::cout << "ARP reply sent successfully." << std::endl;
                }

            } 
            else if (ntohs(arp->ar_op) == ARPOP_REPLY) 
            {
                std::cout << "ARP Reply recieved ";
            } else 
            {
                std::cout << "Got unknown ARP operation ";
            }
        }
    }
    return 0;
}