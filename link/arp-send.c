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

void EtherHeaderCreate(unsigned char *buf,DeviceInfo *dvif)
{
	struct ether_header *eth;
	eth=(unsigned char *)buf;
	
	char broadmac[6]={0xff,0xff,0xff,0xff,0xff,0xff};

	memcpy(eth->ether_shost,dvif->mac,6);
	memcpy(eth->ether_dhost,broadmac,6);
	eth->ether_type=htons(ETH_P_ARP);
}

void ArpReqPacketCreate(unsigned char *buf,DeviceInfo *dvif,char *dstip)
{
	char ArpReqMac[6]={0,0,0,0,0,0};

	ArpPacket *arp;
	arp=buf;
	
	arp->hwtype=htons(1);
	arp->protype=htons(ETH_P_IP);
	arp->hwlen=6;
	arp->prolen=4;
	arp->op=htons(1);
	memcpy(arp->src_mac,dvif->mac,6);
	memcpy(arp->src_ip,dvif->ip,4);
	memcpy(arp->dst_mac,ArpReqMac,6);
	memcpy(arp->dst_ip,dstip,4);
}

void ArpReqSend(unsigned char *packet,DeviceInfo *dvif)
{
	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));

	char mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};

	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=dvif->index;
	sll.sll_protocol=htons(ETH_P_ARP);
	memcpy(sll.sll_addr,mac,6);

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP));


	while(1)
	{
		int r=sendto(fd,packet,sizeof(ArpPacket)+sizeof(struct ether_header),0,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
		if(r<0)
		{
			puts("error");
		}
		else
		{
			puts("send one packet");
		}
		sleep(1);
	}

}

int main()
{
	DeviceInfo dvif;
	memset(&dvif,0,sizeof(DeviceInfo));

	puts("input device name:");
	scanf("%s",dvif.name);
	
	GetDeviceInfo(&dvif);
	PrintDeviceInfo(&dvif);


	unsigned char packet[1024];
	memset(packet,0,1024);
	char dst_ip[4]={0xc0,0xa8,0x1,0x67};
	EtherHeaderCreate(packet,&dvif);
	ArpReqPacketCreate(packet+sizeof(struct ether_header),&dvif,dst_ip);

	for(int i=0;i<sizeof(struct ether_header)+sizeof(ArpPacket);i++)
	{
		if(i%16==0)
		{
			puts("");
		}
		printf("%02X ",packet[i]);
	}
	puts("");

	ArpReqSend(packet,&dvif);
}
