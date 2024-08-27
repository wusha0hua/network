#include <netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa//inet.h>
#include<netinet/ip.h>
#include<netinet/udp.h>

#define LEN 1024

struct UDP_PSD_Header
{
	u_int32_t src_ip;
	u_int32_t dst_ip;
	u_int8_t zero;
	u_int8_t protocol;
	u_int16_t udp_len;
};

unsigned short CheckSum(char *buf,int len)
{
	unsigned long sum=0;
	while(len>0)
	{
		sum+=*buf++;
		len--;
	}
	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	return (unsigned short)(~sum);
}

int main()
{
	char src_ip[15],dst_ip[15];
	int src_port,dst_port;

	puts("input src ip:");
	scanf("%s",src_ip);
	puts("input src_port:");
	scanf("%d",&src_port);
	puts("input dst ip:");
	scanf("%s",dst_ip);
	puts("input dst port:");
	scanf("%d",&dst_port);

	int sock=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
	if(sock<0)
	{
		puts("socket error");
		exit(-1);
	}
	const int val=1;
	if(setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&val,sizeof(int)))
	{
		puts("set socket error");
		exit(-1);
	}

	struct sockaddr_in sin,din;
	memset(&sin,0,sizeof(struct sockaddr_in));
	memset(&din,0,sizeof(struct sockaddr_in));

	sin.sin_addr.s_addr=inet_addr(src_ip);
	sin.sin_port=htons(src_port);
	sin.sin_family=AF_INET;
	din.sin_addr.s_addr=inet_addr(dst_ip);
	din.sin_port=htons(dst_port);
	din.sin_family=AF_INET;

	char packet[LEN];
	memset(packet,0,LEN);
	char data[]="1234567890";
	struct iphdr *ip=(struct iphdr *)packet;
	struct udphdr *udp=(struct udphdr *)(packet+sizeof(struct iphdr));

	ip->version=4;
	ip->ihl=5;
	ip->tos=0;
	ip->tot_len=sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(data);
	ip->ttl=255;
	ip->protocol=IPPROTO_UDP;
	ip->check=0;
	ip->saddr=inet_addr(src_ip);
	ip->daddr=inet_addr(dst_ip);

	udp->source=htons(src_port);
	udp->dest=htons(dst_port);
	udp->len=htons(sizeof(struct udphdr)+sizeof(data));

	char checkdata[sizeof(struct UDP_PSD_Header)+sizeof(struct udphdr)+sizeof(data)];
	memset(checkdata,0,sizeof(checkdata));

	struct UDP_PSD_Header *pseudo=(struct UDP_PSD_Header *)checkdata;
	pseudo->src_ip=inet_addr(src_ip);
	pseudo->dst_ip=inet_addr(dst_ip);
	pseudo->zero=0;
	pseudo->protocol=IPPROTO_UDP;
	pseudo->udp_len=htons(sizeof(struct udphdr)+sizeof(data));
	
	memcpy(checkdata+sizeof(struct UDP_PSD_Header),udp,sizeof(struct udphdr));
	memcpy(checkdata+sizeof(struct UDP_PSD_Header)+sizeof(struct udphdr),data,sizeof(data));


	udp->check=CheckSum(checkdata,sizeof(struct udphdr)+sizeof(struct UDP_PSD_Header)+sizeof(data));

	memcpy(packet+sizeof(struct iphdr)+sizeof(struct udphdr),data,sizeof(data));

	if(sendto(sock,packet,ip->tot_len,0,(struct sockaddr*)&din,sizeof(struct sockaddr))<0)
	{
		puts("error");
		exit(-1);
	}
	else 
	{
		puts("OK");
	}

}
