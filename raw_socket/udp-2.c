#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<arpa/inet.h>

#define LEN 1024

struct UDP_PSD_Header
{
	u_int32_t src_ip;
	u_int32_t dst_ip;
	u_int8_t zero;
	u_int8_t protocol;
	u_int8_t udp_len;
};

unsigned short CheckSum(char *buf,int len)
{
	unsigned long sum;
	for(sum=0;len>0;len--)
	{
		sum+=*buf++;
	}
	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	return (unsigned short)(~sum);
}

int main()
{
	/*char src_ip[]="192.168.1.117";
	char dst_ip[]="192.168.1.117";
	int src_port=65535;
	int dst_port=65535;*/

	char src_ip[15],dst_ip[15];
	int src_port,dst_port;

	memset(src_ip,0,sizeof(src_ip));
	memset(dst_ip,0,sizeof(dst_ip));

	puts("input src ip:");
	scanf("%s",src_ip);
	puts("input src port:");
	scanf("%d",&src_port);
	puts("input dst ip:");
	scanf("%s",dst_ip);
	puts("input dst_port:");
	scanf("%d",&dst_port);

	char data[10]="0123456789";
	char packet[LEN];
	memset(packet,0,LEN);

	struct iphdr *ip=(struct iphdr*)packet;
	struct udphdr *udp=(struct udphdr*)(packet+sizeof(struct iphdr));

	struct sockaddr_in sin,din;
	memset(&sin,0,sizeof(struct sockaddr_in));
	memset(&din,0,sizeof(struct sockaddr_in));

	int sock=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
	if(sock<0)
	{
		puts("socket create fail");
		exit(-1);
	}
	
	sin.sin_addr.s_addr=inet_addr(src_ip);
	sin.sin_port=htons(src_port);
	sin.sin_family=AF_INET;

	din.sin_addr.s_addr=inet_addr(dst_ip);
	din.sin_port=htons(dst_port);
	din.sin_family=AF_INET;

	ip->version=4;
	ip->ihl=5;
	ip->tos=0;
	ip->tot_len=sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(data);
	ip->ttl=64;
	ip->protocol=IPPROTO_UDP;
	ip->check=0;
	ip->saddr=inet_addr(src_ip);
	ip->daddr=inet_addr(dst_ip);

	udp->source=htons(src_port);
	udp->dest=htons(dst_port);
	udp->len=htons(sizeof(struct udphdr)+sizeof(data));

	char *UDPCheckSumData[sizeof(struct UDP_PSD_Header)+sizeof(struct udphdr)+sizeof(data)];
	memset(UDPCheckSumData,0,sizeof(UDPCheckSumData));

	struct UDP_PSD_Header *pseudo=(struct UDP_PSD_Header *)UDPCheckSumData;
	pseudo->src_ip=inet_addr(src_ip);
	pseudo->dst_ip=inet_addr(dst_ip);
	pseudo->zero=0;
	pseudo->protocol=IPPROTO_UDP;
	pseudo->udp_len=htons(sizeof(struct udphdr)+sizeof(data));

	memcpy(UDPCheckSumData+sizeof(struct UDP_PSD_Header),udp,sizeof(struct udphdr));
	memcpy(UDPCheckSumData+sizeof(struct UDP_PSD_Header)+sizeof(data),data,sizeof(data));

	udp->check=CheckSum((char *)UDPCheckSumData,sizeof(struct udphdr)+sizeof(struct UDP_PSD_Header)+sizeof(data));

	memcpy(packet+sizeof(struct iphdr)+sizeof(struct udphdr),data,sizeof(data));

	if(sendto(sock,packet,ip->tot_len,0,(struct sockaddr *)&din,sizeof(din))<0)
	{
		puts("send fail");
		exit(-1);
	}

	close(sock);
}
