#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "arpa/inet.h"
#include <fcntl.h>
#define REQUEST "IP: %s - MAC: %02X:%02X:%02X:%02X:%02X:%02X\n"

typedef struct arp_message
{
	unsigned char dest_mac[6]; // eth
	unsigned char src_mac[6]; // eth
	unsigned short type; // eth
	unsigned short hardware_type; // arp
	unsigned short protocol_type; // arp
	unsigned char hardware_adress_size; // arp
	unsigned char protocol_adress_size; // arp
	unsigned short opcode; // arp
	unsigned char sender_mac[6]; // arp
	unsigned char sender_ip[4]; // arp
	unsigned char target_mac[6]; // arp
	unsigned char target_ip[4]; // arp
} arp_message_t;

void print_msg(arp_message_t msg)
{
	printf("Destination MAC: %x.%x.%x.%x.%x.%x\n", msg.dest_mac[0], msg.dest_mac[1], msg.dest_mac[2], msg.dest_mac[3], msg.dest_mac[4], msg.dest_mac[5]);
	printf("Source MAC: %x.%x.%x.%x.%x.%x\n", msg.src_mac[0], msg.src_mac[1], msg.src_mac[2], msg.src_mac[3], msg.src_mac[4], msg.src_mac[5]);
	printf("Type: %d\n\n", msg.type);

	printf("Hardware type: %x\n", msg.hardware_type);
	printf("Protocol type: %x\n", msg.protocol_type);
	printf("Hardware address size: %d\n", msg.hardware_adress_size);
	printf("Protocol address size: %d\n", msg.protocol_adress_size);
	printf("Opcode: %d\n", msg.opcode);
	printf("Sender MAC: %x.%x.%x.%x.%x.%x\n", msg.sender_mac[0], msg.sender_mac[1], msg.sender_mac[2], msg.sender_mac[3], msg.sender_mac[4], msg.sender_mac[5]);
	printf("Sender IP: %d.%d.%d.%d\n", msg.sender_ip[0], msg.sender_ip[1], msg.sender_ip[2], msg.sender_ip[3]);
	printf("Target MAC: %x.%x.%x.%x.%x.%x\n", msg.target_mac[0], msg.target_mac[1], msg.target_mac[2], msg.target_mac[3], msg.target_mac[4], msg.target_mac[5]);
	printf("Target IP: %d.%d.%d.%d\n\n", msg.target_ip[0], msg.target_ip[1], msg.target_ip[2], msg.target_ip[3]);
}

int main(int argc, char *argv[])
{
	struct ifreq ifr;
	struct sockaddr socket_address;
	unsigned char ip[4];
	int sock, optval, n;
	arp_message_t msg;

	int fd=open("arp_read_log.txt", O_WRONLY|O_APPEND, 0666);
	dup2(fd, 1);
	// Must be run with 2 arguments
	if (argc != 3)
  	return -1;

	// Opening socket
	if((sock = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0)
	{
		perror("Error! Socket cannot be opened.");
		return -1;
	}

	// Set options on sockets
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
	{
		perror("Error! 'setsockopt' function did not work.");
		return -1;
	}

	// Creating package

	// Interface
	strcpy(ifr.ifr_name, argv[1]);

	// Get sender MAC
	if(ioctl(sock, SIOCGIFHWADDR, &ifr) < 0)
	{
		perror("cant get the sepciefied network interface MAC address");
		return -1;
	}

	// Get sender IP
	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("Error! Sender IP cannot be gotten.");
		return -1;
	}

	char *my_ip;
	memset(my_ip, '\0', 4);

	struct sockaddr_in *addr = (struct sockaddr_in *)&(ifr.ifr_addr);
	my_ip=inet_ntoa(addr->sin_addr);

	printf("inet addr:%s\n",my_ip);
	do
	{
		bzero(&socket_address, sizeof(socket_address));
		bzero(&msg, sizeof(msg));
		n = sizeof(socket_address);

		if(recv(sock, &msg, sizeof(msg), 0)==-1)
		{
			perror("Error!");
			return -1;
		}
		// char *dest_ip;
		// dest_ip=inet_ntoa(msg.target_ip);

		// fprintf(stdout, REQUEST, argv[2],
		// 		msg.sender_mac[0], msg.sender_mac[1], msg.sender_mac[2],
		// 		msg.sender_mac[3], msg.sender_mac[4], msg.sender_mac[5]);

		char dest_ip[INET_ADDRSTRLEN], src_ip[INET_ADDRSTRLEN];
		snprintf(src_ip, INET_ADDRSTRLEN, "%d.%d.%d.%d\n", msg.sender_ip[0], msg.sender_ip[1], msg.sender_ip[2], msg.sender_ip[3]);
		snprintf(dest_ip, INET_ADDRSTRLEN, "%d.%d.%d.%d\n", msg.target_ip[0], msg.target_ip[1], msg.target_ip[2], msg.target_ip[3]);

			printf("%s\n", src_ip);
			printf("%s\n", dest_ip);
			printf("Target MAC: %x.%x.%x.%x.%x.%x\n", msg.target_mac[0], msg.target_mac[1], msg.target_mac[2], msg.target_mac[3], msg.target_mac[4], msg.sender_mac[5]);
			printf("------------------\n");

		sleep(1);

	} while(1);

	close(sock);
	return(0);
}
