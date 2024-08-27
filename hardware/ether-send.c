#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<net/if.h>
#include<linux/if_packet.h>
#include<unistd.h>
#include<arpa/inet.h>

typedef struct
{
	char dst[6];
	char src[6];
	unsigned short type;
}LLDP;

int main()
{
	LLDP *packet=(LLDP*)malloc(sizeof(LLDP));
	memset(packet,0,sizeof(LLDP));

	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));

	int fd=socket(PF_PACKET,SOCK_RAW,htons(0x88cc));

	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=if_nametoindex("wlan0");
	sll.sll_protocol=htons(0x88cc);

	sll.sll_addr[0]=1;
	sll.sll_addr[1]=1;
	sll.sll_addr[2]=1;
	sll.sll_addr[3]=1;
	sll.sll_addr[4]=1;
	sll.sll_addr[5]=1;

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

	packet->type=htons(0x88cc);

	while(1)
	{
		int r=sendto(fd,packet,sizeof(LLDP),0,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
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
