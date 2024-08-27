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
	unsigned char buf[1024];
	struct sockaddr_in sockaddr;

	sock=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP));
	//sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0)
	{
		perror("socket error");
		exit(-1);
	}

	memset(&sockaddr,0,sizeof(struct sockaddr));
	sockaddr.sin_addr.s_addr=inet_addr("192.168.1.117");
	sockaddr.sin_port=htons(65535);
	sockaddr.sin_family=AF_INET;
	unsigned int len;
	len=sizeof(struct sockaddr);

	char mac[6]={0x90,0x78,0x41,0x2d,0xd8,0xa9};
	n=1;
	while(n)
	{
		memset(buf,0,LEN);
		recvlen=recvfrom(sock,buf,LEN,0,NULL,NULL);
		if(memcmp(mac,buf,6)==0)
		{
			printf("%d:",n);
			for(i=0;i<recvlen;i++)
			{
				if(i%16==0)
				{
					puts("");
				}
				printf("%02X ",buf[i]);
			}
		}
	//n--;
	}

	puts("");
	close(sock);
}
