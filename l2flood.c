#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <bits/ioctls.h>
#include <linux/if_arp.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>

#define PACKET_SIZE 60

int fd;
struct sockaddr_ll destination;


void fill_destination(char* interface_name){
	// initialize to 0
	memset(&destination, 0, sizeof(destination));
	// fill interface information
	struct ifreq interface = {0};
	memcpy(interface.ifr_name, interface_name, strlen(interface_name));
	if(ioctl(fd, SIOCGIFINDEX, &interface)){
		perror("Getting index of interface");
		exit(1);
	}
	destination.sll_ifindex = interface.ifr_ifindex;
	if(ioctl(fd, SIOCGIFHWADDR, &interface)){
		perror("Getting mac address of interface");
		exit(2);
	}
	for(int i = 0; i < ETH_ALEN; i++) destination.sll_addr[i] = interface.ifr_hwaddr.sa_data[i];
	destination.sll_addr[6] = 0x00;
	destination.sll_addr[7] = 0x00;

	// fill the rest
	destination.sll_family = AF_PACKET;
	destination.sll_protocol = htons(ETH_P_ALL);
	destination.sll_halen = ETH_ALEN;
}


void initialize_l2_packet(char* packet, char* interface_name){
	fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(fd < 0)
	{
		perror("Error creating raw socket ");
		exit(3);
	}
	struct ethhdr* hdr = (struct ethhdr*) packet;
	hdr->h_proto = htons(ETH_P_ALL);
	for(int i = 0; i < ETH_ALEN; i++) hdr->h_dest[i] = UCHAR_MAX; // Broadcast mac address
	fill_destination(interface_name); // filling socket address for destination
}

void modify_l2_packet(char* packet){
	struct ethhdr* hdr = (struct ethhdr*) packet;
	for(int i = 0; i < ETH_ALEN; i++){
		hdr->h_source[i] = rand() % UCHAR_MAX;
	}
}


void send_l2_packet(char* packet){
	int packetsize = sizeof(struct ethhdr);
	int sent_bytes = sendto(fd, packet, packetsize, 0, (struct sockaddr *)&destination, sizeof(destination));
	if(sent_bytes < packetsize){
		perror("Sending packet");
		exit(4);
	}
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL)); // seed of pseudo number generator
	if(argc < 2){
		fprintf(stderr, "Missing argument: interface name \n");
		exit(5);
	}
	char packet[PACKET_SIZE];
	initialize_l2_packet(packet, argv[1]);
	while(1){
		modify_l2_packet(packet);
		send_l2_packet(packet);
	}
	return 0;
}
