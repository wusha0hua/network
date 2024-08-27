#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<linux/if_packet.h>
#include<net/ethernet.h>

typedef struct 
{
	unsigned char name[10];
	int index;
	unsigned char ip[4];
	unsigned char mac[6];
}DeviceInfo;


typedef struct
{
	unsigned short hwtype;
	unsigned short protype;
	unsigned char hwlen;
	unsigned char prolen;
	unsigned short op;
	unsigned char src_mac[6];
	unsigned char src_ip[4];
	unsigned char dst_mac[6];
	unsigned char dst_ip[4];
}ArpPacket;

void GetDeviceInfo(DeviceInfo *dvif)
{
	struct ifreq ifr;
	memset(&ifr,0,sizeof(struct ifreq));

	int fd=socket(AF_INET,SOCK_DGRAM,0);

	memcpy(ifr.ifr_ifrn.ifrn_name,dvif->name,sizeof(struct ifreq));

	if(ioctl(fd,SIOCGIFADDR,&ifr)==-1)
	{
		exit(-1);
	}

	memcpy(dvif->ip,ifr.ifr_ifru.ifru_addr.sa_data+2,4);
	
	if(ioctl(fd,SIOCGIFHWADDR,&ifr)==-1)
	{
		exit(-1);
	}

	memcpy(dvif->mac,ifr.ifr_ifru.ifru_hwaddr.sa_data,6);

	if(ioctl(fd,SIOCGIFINDEX,&ifr)==-1)
	{
		exit(-1);
	}

	dvif->index=ifr.ifr_ifindex;
}

void PrintDeviceInfo(DeviceInfo *dvif)
{
	printf("name:%s\n",dvif->name);
	printf("ip:%d.%d.%d.%d\n",dvif->ip[0],dvif->ip[1],dvif->ip[2],dvif->ip[3]);
	printf("mac:%02X.%02X.%02X.%02X.%02X.%02X\n",dvif->mac[0],dvif->mac[1],dvif->mac[2],dvif->mac[3],dvif->mac[4],dvif->mac[5]);
	printf("index:%d\n",dvif->index);
}

void ArpAnalysis(unsigned char *packet)
{
	struct ether_header *eth;
	ArpPacket *arp;
	eth=packet;
	arp=(packet+sizeof(struct ether_header));

	printf("ether:\n");
	printf("\tdst_mac:%02X.%02X.%02X.%02X.%02X.%02X\n",eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5]);
	printf("\tsrc_mac:%02X.%02X.%02X.%02X.%02X.%02X\n",eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],eth->ether_shost[4],eth->ether_shost[5]);

	printf("arp:\n");
	printf("\thardware type:%d\n",ntohs(arp->hwtype));
	printf("\tprotocol type:0x%04X\n",ntohs(arp->protype));
	printf("\thardware address length:%d\n",arp->hwlen);
	printf("\top:%d\n",ntohs(arp->op));
	printf("\tsrc mac address:%02X.%02X.%02X.%02X.%02X.%02X\n",arp->src_mac[0],arp->src_mac[1],arp->src_mac[2],arp->src_mac[3],arp->src_mac[4],arp->src_mac[5]);
	printf("\tsrc ip address%d.%d.%d.%d\n",arp->src_ip[0],arp->src_ip[1],arp->src_ip[2],arp->src_ip[3]);
	printf("\tdst mac address:%02X.%02X.%02X.%02X.%02X.%02X\n",arp->dst_mac[0],arp->dst_mac[1],arp->dst_mac[2],arp->dst_mac[3],arp->dst_mac[4],arp->dst_mac[5]);
	printf("\tdst ip address%d.%d.%d.%d\n",arp->dst_ip[0],arp->dst_ip[1],arp->dst_ip[2],arp->dst_ip[3]);
	
	puts("");
}


int main()
{
	DeviceInfo dvif;
	memset(&dvif,0,sizeof(DeviceInfo));

	puts("input device name:");
	scanf("%s",dvif.name);
	
	GetDeviceInfo(&dvif);
	PrintDeviceInfo(&dvif);

	unsigned char buffer[1500];

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP));

	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));

	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=dvif.index;
	sll.sll_protocol=htons(ETH_P_ARP);


	char mac[6]={0x90,0x78,0x41,0x2d,0xd8,0xa9};
	//memcpy(sll.sll_addr,mac,6);

	while(1)
	{
		memset(buffer,0,sizeof(buffer));
		int r=recvfrom(fd,buffer,1024,0,(struct sockaddr*)&sll,sizeof(struct sockaddr));
		
		if(memcmp(buffer,mac,6)==0)
		{

			ArpAnalysis(buffer);
			/*for(int i=0;i<sizeof(struct ether_header)+sizeof(ArpPacket);i++)
			{
				if(i%16==0)
				{
					puts("");
				}
				printf("%02X ",buffer[i]);
			}
			puts("");*/
		}
	}
}
