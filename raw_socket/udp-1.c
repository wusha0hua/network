#include <netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/udp.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>

#define LEN 1024

struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

unsigned short csum(unsigned short *ptr,int nbytes)
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1)
	{
		sum+=*ptr++;
		nbytes-=2;
	}

	if(nbytes==1)
	{
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum=(sum>>16)+(sum&0xffff);
	sum=sum+(sum>>16);
	answer=(short)~sum;

	return (answer);
}

int main()
{
	int sock=socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
	
	char ippacket[LEN],udppacket[LEN];
	memset(ippacket,0,LEN);
	memset(udppacket,0,LEN);

	struct iphdr *iph=(struct iphdr *)ippacket;
	struct udphdr *udph=(struct udphdr *)(ippacket+sizeof(struct ip));

	char *data=ippacket+sizeof(struct iphdr)+sizeof(struct udphdr);

	strcpy(data,"1234567890");

	struct pseudo_header psh;
	char ip_addr[]="192.168.1.117";
	int port=65535;

	struct sockaddr_in sin;
	memset(&sin,0,sizeof(struct sockaddr_in));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(port);
	sin.sin_addr.s_addr=inet_addr(ip_addr);

	iph->ihl=5;
	iph->version=4;
	iph->tos=0;
	iph->tot_len=sizeof(struct iphdr)+sizeof(struct udphdr)+1300;
	iph->id=htonl(65555);
	iph->frag_off=0;
	iph->ttl=255;
	iph->protocol=IPPROTO_UDP;
	iph->check=0;
	iph->saddr=inet_addr(ip_addr);
	iph->daddr=sin.sin_addr.s_addr;
	iph->check=0;

	psh.source_address=inet_addr(ip_addr);
	psh.dest_address=sin.sin_addr.s_addr;
	psh.placeholder=0;
	psh.protocol=IPPROTO_UDP;
	psh.udp_length=htons(sizeof(struct udphdr)+1300);

	int psize=sizeof(struct pseudo_header)+sizeof(struct udphdr)+1300;
	char *pseudodata=malloc(psize);
	
	memcpy(pseudodata,(char*)&psh,sizeof(struct pseudo_header));
	memcpy(pseudodata+sizeof(struct pseudo_header),udph,sizeof(struct udphdr)+1300);

	udph->check=csum((unsigned short*)pseudodata,psize);

	//while(1)
	{
		if(sendto(sock,ippacket,iph->tot_len,0,(struct sockaddr*)&sin,sizeof(sin))<0)
		{
			puts("error");
		}
	}

	return 0;
}
