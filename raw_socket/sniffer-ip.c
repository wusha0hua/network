#include <linux/if_ether.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<arpa/inet.h>
#include<netinet/ether.h>

#define LEN 1024

int main()
{
	int sock,n,i;
	ssize_t recvlen;
	unsigned char packet[1024];
	struct sockaddr_in sockaddr;

	sock=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_IP));
	//sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0)
	{
		puts("socket error");
		exit(-1);
	}

	memset(&sockaddr,0,sizeof(struct sockaddr));
	sockaddr.sin_addr.s_addr=inet_addr("192.168.1.117");
	sockaddr.sin_port=htons(65535);
	sockaddr.sin_family=AF_INET;
	unsigned int len;
	len=sizeof(sockaddr);

	n=1;
	int j=1;
	int l,k;
	unsigned char *p;
	unsigned short byte2;
	unsigned short byte4;
	while(j<n+1)
	{
		memset(packet,0,LEN);
		recvlen=recvfrom(sock,packet,LEN,0,NULL,NULL);
			
		printf("%d :\n",j);
		puts("ether :");
		
		p=packet;
		printf("\tdestination MAC address : ");
		for(k=0;k<6;k++)
		{
			printf("%02X ",p[k]);
		}
		puts("");

		p=p+6;
		printf("\tsource MAC address : ");
		for(k=0;k<6;k++)
		{
			printf("%02X ",p[k]);
		}
		puts("");

		p=p+6;
		printf("\tprotocol : ");
		for(k=0;k<2;k++)
		{
			printf("%02X ",p[k]);
		}
		puts("");
		
		puts("ip : ");

		p+=2;
		printf("\tversion : %02X\n",((*p)>>4)&15);

		printf("\tihl : %02X\n",*p&15);

		p++;
		printf("\ttos : %02X\n",*p);

		p++;
		byte2=(*p)<<8|*(p+1);
		printf("\ttot_len : %02X\n",byte2);

		p+=2;
		byte2=(*p)<<8|*(p+1);
		printf("\tid : %02X",byte2);

		p+=2;
		byte2=(*p)<<8|*(p+1);
		printf("\tfrag_off : %02X\n",byte2);

		p+=2;
		printf("\tttl : %02X\t",*p);

		p++;
		printf("\tprotocol : %02X\n",*p);

		p++;
		byte2=(*p)<<8|*(p+1);
		printf("\tcheck : %02X\n",byte2);

		p+=2;
		printf("\tsaddr : %d.%d.%d.%d\n",*p,*(p+1),*(p+2),*(p+3));

		p+=4;
		printf("\tdaddr : %d.%d.%d.%d\n",*p,*(p+1),*(p+2),*(p+3));

		for(i=0;i<recvlen;i++)
		{
			if(i%16==0)
			{
				puts("");
			}
			printf("%02X ",packet[i]);
		}
	n--;
	}

	puts("");
}
