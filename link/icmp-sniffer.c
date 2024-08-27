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
#include<netinet/ip_icmp.h>

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


void IcmpPacketAnalysis(char *packet)
{
	struct ether_header *eth;
	struct iphdr *ip;
	struct icmp *icmp; 

	eth=packet;
	ip=packet+sizeof(struct ether_header);
	icmp=packet+sizeof(struct ether_header)+sizeof(struct iphdr);

	printf("ether:\n");
	printf("\tdst mac:%02X.%02X.%02X.%02X.%02X.%02X\n",eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5]);
	printf("\tsrc mac:%02X.%02X.%02X.%02X.%02X.%02X\n",eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],eth->ether_shost[4],eth->ether_shost[5]);

	printf("ip:\n");
	printf("\tversion:%d\n",ip->version);
	printf("\tihl:%d\n",ip->ihl);
	printf("\ttos:%d\n",ip->tos);
	printf("\ttotal len:%d\n",ip->tot_len);
	printf("\tid:%d\n",ip->id);
	printf("\tflag and offset:%d\n",ip->frag_off);
	printf("\tttl:%d\n",ip->ttl);
	printf("\tprotocol:%d\n",ip->protocol);
	printf("\tcheck:%d(%d)\n",ip->check,Check((unsigned short *)ip,10));
	
	char buf[16];
	memset(buf,0,16);
	inet_ntop(AF_INET,&ip->saddr,buf,sizeof(buf));
	printf("\tsrc ip:%s\n",buf);
	memset(buf,0,16);
	inet_ntop(AF_INET,&ip->daddr,buf,sizeof(buf));
	printf("\tdst ip:%s\n",buf);

	printf("icmp:\n");
	printf("\ttype:%d\n",icmp->icmp_type);
	printf("\tcode:%d\n",icmp->icmp_code);
	printf("\tchecksum:%d(%d)\n",icmp->icmp_cksum,Check((unsigned short*)icmp,sizeof(struct icmp)/2));
	printf("\tid:%d\n",icmp->icmp_hun.ih_idseq.icd_id);
	printf("\tseq:%d\n",icmp->icmp_hun.ih_idseq.icd_seq);


	for(int i=0;i<sizeof(struct icmp);i++)
	{
		if(i%16==0)
		{
			puts("");
		}
	
		printf("%02X ",*((unsigned char*)(icmp)+i));
	}

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


	unsigned char packet[1024];
	
	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));

	sll.sll_family=PF_PACKET;
	sll.sll_protocol=htons(ETH_P_IP);
	sll.sll_ifindex=dvif.index;
	
	char buffer[1024];
	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	while(1)
	{
		memset(buffer,0,sizeof(buffer));
		if(recvfrom(fd,buffer,1024,0,NULL,NULL)<0)
		{
			puts("error");
		}
		else
		{
			struct iphdr *ip=buffer+sizeof(struct ether_header);
			if(ip->protocol==IPPROTO_ICMP)
			{
				IcmpPacketAnalysis(buffer);	
			}
			else
			{
				//printf("%d %d\n",ip->protocol,IPPROTO_ICMP);
			}
		}
		sleep(1);

	}

	close(fd);
}
