#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<net/if.h>
#include<linux/if_packet.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<net/ethernet.h>

typedef struct
{
	char dst[6];
	char src[6];
	unsigned short type;
	
	unsigned short hwtype;
	unsigned short pro;
	unsigned char hwlen;
	unsigned char prolen;
	unsigned short op;
	unsigned char src_mac[6];
	unsigned char src_ip[4];
	unsigned char dst_mac[6];
	unsigned char dst_ip[4];
}ARP;

int main()
{
	ARP *packet=(ARP*)malloc(sizeof(ARP));
	memset(packet,0,sizeof(ARP));

	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP));

	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=if_nametoindex("wlan0");
	sll.sll_protocol=htons(ETH_P_ARP);

	sll.sll_addr[0]=4;
	sll.sll_addr[1]=4;
	sll.sll_addr[2]=4;
	sll.sll_addr[3]=4;
	sll.sll_addr[4]=4;
	sll.sll_addr[5]=4;

	packet->dst[0]=1;
	packet->dst[1]=1;
	packet->dst[2]=1;
	packet->dst[3]=1;
	packet->dst[4]=1;
	packet->dst[5]=1;

	packet->src[0]=2;
	packet->src[1]=2;
	packet->src[2]=2;
	packet->src[3]=2;
	packet->src[4]=2;
	packet->src[5]=2;

	packet->type=htons(ETH_P_ARP);

	packet->hwtype=1;
	packet->hwlen=6;
	packet->pro=htons(ETH_P_ARP);
	packet->prolen=4;
	packet->op=1;
	memcpy(packet->src_mac,"\3\3\3\3\3\3",6);
	memcpy(packet->src_ip,"\3\3\3\3",4);
	memcpy(packet->dst_mac,"\3\3\3\3\3\3",6);
	memcpy(packet->dst_ip,"\3\3\3\3",4);

	while(1)
	{
		int r=sendto(fd,packet,sizeof(ARP),0,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
		if(r<0)
		{
			puts("error");
		}
		else
		{
			puts("send");
		}
		sleep(1);
	}
}
