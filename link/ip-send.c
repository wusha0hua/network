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
#include<netinet/ip.h>

typedef struct 
{
	unsigned char name[10];
	int index;
	unsigned char ip[4];
	unsigned char mac[6];
}DeviceInfo;



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

unsigned short Check(unsigned short *buf,int len)
{
	unsigned int sum=0;
	for(int i=0;i<len;i++)
	{
		sum+=buf[i];
	}

	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	return ~sum;
}

void EtherHeaderCreate(unsigned char *buf,DeviceInfo *dvif)
{
	struct ether_header *eth;
	eth=(unsigned char *)buf;
	
	char broadmac[6]={0xff,0xff,0xff,0xff,0xff,0xff};

	memcpy(eth->ether_shost,dvif->mac,6);
	memcpy(eth->ether_dhost,dvif->mac,6);
	eth->ether_type=htons(ETH_P_IP);
}

void IPPacketCreate(unsigned char *buf,DeviceInfo *dvif)
{
	struct iphdr *ip;
	ip=buf;

	ip->version=4;
	ip->ihl=5;
	ip->tos=0;
	ip->tot_len=htons(20);
	ip->id=1;
	ip->frag_off=0;
	ip->ttl=255;
	ip->protocol=0;
	ip->check=0;
	ip->saddr=(dvif->ip[3])|(dvif->ip[2]<<8)|(dvif->ip[1]<<16)|(dvif->ip[0]<<24);
	ip->daddr=(dvif->ip[0])|(dvif->ip[1]<<8)|(dvif->ip[2]<<16)|(dvif->ip[3]<<24);
	
	ip->check=(Check((unsigned short*)ip,10));

}

void IPSend(unsigned char *packet,DeviceInfo *dvif)
{
	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));


	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=dvif->index;
	sll.sll_protocol=htons(ETH_P_IP);
	memcpy(sll.sll_addr,dvif->mac,6);

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	printf("fd: %d\n", fd);


	while(1)
	{
		int r=sendto(fd,packet,sizeof(struct iphdr)+sizeof(struct ether_header),0,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
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
	IPPacketCreate(packet+sizeof(struct ether_header),&dvif);

	for(int i=0;i<sizeof(struct ether_header)+sizeof(struct iphdr);i++)
	{
		if(i%16==0)
		{
			puts("");
		}
		printf("%02X ",packet[i]);
	}
	puts("");

	IPSend(packet,&dvif);
}
