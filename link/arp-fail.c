#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<sys/ioctl.h>
#include<netinet/if_ether.h>
#include<net/if.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>

typedef struct
{
	unsigned short hrd;
	unsigned short pro;
	unsigned char hln;
	unsigned char pln;
	unsigned short op;
	unsigned char sha[6];
	unsigned char *sip;
	unsigned char dha[6];
	unsigned char *dip;
}ARP;


int main()
{
	char packet[ETH_FRAME_LEN];
	memset(&packet,0,ETH_FRAME_LEN);

	char eth_dha[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	char arp_dha[6]={0,0,0,0,0,0};
	char sha[6]={1,1,1,1,1,1};

	char dip[4]={1,1,1,1};
	char sip[4]={2,2,2,2};

	int fd=socket(AF_INET,SOCK_PACKET,htons(0x0003));
	
	struct ethhdr *eth;
	ARP *arp;

	eth=(struct ethhdr *)packet;
	arp=(ARP*)(packet+sizeof(struct ethhdr));

	memcpy(eth->h_dest,eth_dha,6);
	memcpy(eth->h_source,sha,6);
	eth->h_proto=htons(0x0806);

	arp->hrd=htons(1);
	arp->pro=htons(0x0800);
	arp->hln=6;
	arp->pln=4;

	memcpy(arp->sha,sha,6);
	arp->sip=(unsigned char *)inet_addr("1.1.1.1");
	memcpy(arp->dha,eth_dha,6);
	arp->dip=(unsigned char *)inet_addr("2.2.2.2");

	int i=0;
	for(i=0;i<10;i++)
	{
		printf("%d\n",i);
		write(fd,packet,ETH_FRAME_LEN);
		sleep(1);
	}
}
