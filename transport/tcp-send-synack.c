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
#include<netinet/tcp.h>

typedef struct 
{
	unsigned char name[10];
	int index;
	unsigned char ip[4];
	unsigned char mac[6];
}DeviceInfo;

typedef struct
{
	unsigned char src_ip[4];
	unsigned char dst_ip[4];
	unsigned char zero;
	unsigned char protocol;
	unsigned short len;
}TcpPseudoHeader;

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

void IPPacketCreate(unsigned char *buf,DeviceInfo *dvif,char *data)
{
	struct iphdr *ip;
	ip=buf;

	ip->version=4;
	ip->ihl=5;
	ip->tos=0;
	ip->tot_len=htons(sizeof(struct iphdr)+sizeof(struct tcphdr)+sizeof(data));
	ip->id=1;
	ip->frag_off=0;
	ip->ttl=255;
	ip->protocol=IPPROTO_TCP;
	ip->check=0;
	ip->saddr=(dvif->ip[0])|(dvif->ip[1]<<8)|(dvif->ip[2]<<16)|(dvif->ip[3]<<24);
	ip->daddr=(dvif->ip[0])|(dvif->ip[1]<<8)|(dvif->ip[2]<<16)|(dvif->ip[3]<<24);
	
	ip->check=(Check((unsigned short*)ip,10));

}

void TcpPacketCreate(unsigned char *buf,DeviceInfo *dvif,char *data)
{
	struct tcphdr *tcp;
	TcpPseudoHeader *pseudo;

	int src_port=65535;
	int dst_port=65535;

	int chklen=sizeof(struct tcphdr)+sizeof(TcpPseudoHeader)+sizeof(data);
	char *chkbuf=(char*)malloc(chklen);
	memset(chkbuf,0,chklen);

	pseudo=chkbuf;
	tcp=chkbuf+sizeof(TcpPseudoHeader);
	memcpy(chkbuf+sizeof(TcpPseudoHeader)+sizeof(struct tcphdr),data,sizeof(data));

	memcpy(pseudo->src_ip,dvif->ip,4);
	memcpy(pseudo->dst_ip,dvif->ip,4);
	pseudo->protocol=IPPROTO_TCP;
	pseudo->zero=0;
	pseudo->len=htons(sizeof(struct tcphdr)+sizeof(data));

	memcpy(chkbuf+sizeof(TcpPseudoHeader)+sizeof(struct tcphdr),data,sizeof(data));

	tcp->th_sport=htons(src_port);
	tcp->th_dport=htons(dst_port);
	tcp->th_seq=10;
	tcp->th_ack=0;
	tcp->th_off=5;;
	tcp->th_flags=TH_ACK|TH_SYN;
	tcp->th_win=1111;
	tcp->th_sum=0;
	tcp->th_urp=0;

	tcp->th_sum=Check((unsigned short*)chkbuf,chklen/2);
	
	memcpy(buf,tcp,sizeof(struct tcphdr));
	memcpy(buf+sizeof(struct tcphdr),data,sizeof(data));

}

void AppendData(char *packet,char *data)
{
	memcpy(packet+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct tcphdr),data,sizeof(data));
}

void TcpPacketSend(unsigned char *packet,DeviceInfo *dvif,char *data)
{
	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));


	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=dvif->index;
	sll.sll_protocol=htons(ETH_P_IP);
	memcpy(sll.sll_addr,dvif->mac,6);

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));


//	while(1)
	{
		int r=sendto(fd,packet,sizeof(struct iphdr)+sizeof(struct ether_header)+sizeof(struct tcphdr)+sizeof(data),0,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
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
	char data[]="";
	memset(packet,0,1024);
	char dst_ip[4]={0xc0,0xa8,0x1,0x67};
	EtherHeaderCreate(packet,&dvif);
	IPPacketCreate(packet+sizeof(struct ether_header),&dvif,data);
	TcpPacketCreate(packet+sizeof(struct ether_header)+sizeof(struct iphdr),&dvif,data);

	for(int i=0;i<sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct tcphdr)+sizeof(data);i++)
	{
		if(i%16==0)
		{
			puts("");
		}
		printf("%02X ",packet[i]);
	}
	puts("");

	TcpPacketSend(packet,&dvif,data);
}
