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

	unsigned char vl;
	unsigned char tot;
	unsigned short totlen;
	unsigned short id;
	unsigned short flgoff;
	unsigned char ttl;
	unsigned char pro;
	unsigned short chk;
	unsigned char src_ip[4];
	unsigned char dst_ip[4];

}IP;

unsigned short checksum(unsigned short *buffer, int len)
{
	register int nleft = len;
	register unsigned short *w = buffer;
	register unsigned short answer;
	register int sum = 0;
	// 使用32bit的累加器，进行16bit的反馈计算
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}
	// 补全奇数位
	if (nleft == 1) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w;
		sum += u;
	}
	// 将反馈的16bit从高位移至地位
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);					/* add carry */
	answer = ~sum;						/* truncate to 16 bits */
	return (answer);
}


int main()
{
	IP *packet=(IP*)malloc(sizeof(IP));
	memset(packet,0,sizeof(IP));

	struct sockaddr_ll sll;
	memset(&sll,0,sizeof(struct sockaddr_ll));

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));

	sll.sll_family=PF_PACKET;
	sll.sll_ifindex=if_nametoindex("wlan0");
	sll.sll_protocol=htons(ETH_P_IP);

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

	packet->type=htons(ETH_P_IP);

	packet->vl=((char)(4)<<4)|(5&(15));
	packet->tot=0;
	packet->totlen=htons(5+20);
	packet->id=1;
	packet->flgoff=0;
	packet->ttl=255;
	packet->pro=htons(ETH_P_IP);
	packet->chk=0;
	memcpy(packet->src_ip,"\0\0\0\0\0\0",6);
	memcpy(packet->dst_ip,"\0\0\0\0\0\0",6);

	packet->chk=checksum((unsigned short*)(packet+14),(int)sizeof(IP)-14);

	while(1)
	{
		int r=sendto(fd,packet,sizeof(IP),0,(struct sockaddr *)&sll,sizeof(struct sockaddr_ll));
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
