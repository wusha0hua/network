#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<linux/if.h>
#include<sys/ioctl.h>
#include<linux/if_packet.h>
#include<linux/if_ether.h>
#include<errno.h>

int get_eth_MAC(char *eth_name,unsigned char *MAC)
{
	struct ifreq ifr;
	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		puts("socket error");
		exit(0);
	}

	strncpy(ifr.ifr_ifrn.ifrn_name,(char*)eth_name,sizeof(ifr.ifr_ifrn));
	
	int ret_ioctl=ioctl(fd,SIOCGIFHWADDR,&ifr);

	if(ret_ioctl<0)
	{
		puts("ioctl error");
		exit(-1);
	}

	int i=0;
	/*for(i=0;i<14;i++)
	{
		printf("%02X\t",(unsigned char)ifr.ifr_ifru.ifru_hwaddr.sa_data[i]);
	}
	
	puts("");
*/
	memcpy(MAC,ifr.ifr_ifru.ifru_hwaddr.sa_data,6);
	close(fd);
	return 0;
}

typedef struct 
{
	unsigned char dst_mac[6];
	unsigned char src_mac[6];
	unsigned short pro;

	unsigned short arp_hwtype;
	unsigned short arp_protype;
	unsigned char arp_hwlen;
	unsigned char arp_prolen;
	unsigned short arp_op;
	unsigned char arp_src_mac[6];
	unsigned char arp_src_ip[4];
	unsigned char arp_dst_mac[6];
	unsigned char arp_dst_ip[4];
}PACKET;

int main()
{
	char device[20];
	unsigned char local_mac[6];
	puts("input the device name:");
	scanf("%s",device);
	get_eth_MAC(device,local_mac);

	/*int i=0;
	for(i=0;i<6;i++)
	{
		printf("%02X ",local_mac[i]);
	}
	puts("");*/

	PACKET packet;
	
	unsigned char eth_mac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	unsigned char dst_ip[4]={1,1,1,1};

	//unsigned char loacl_mac[6]={1,1,1,1,1,1};
	unsigned char local_ip[4]={0xC0,0xA8,0x01,0x76};
	unsigned char arp_mac[6]={0,0,0,0,0,0};

	memcpy(packet.src_mac,local_mac,6);
	memcpy(packet.dst_mac,eth_mac,6);
	packet.pro=htons(ETH_P_ARP);

	packet.arp_hwtype=htons(1);
	packet.arp_protype=htons(ETH_P_ARP);
	packet.arp_hwlen=6;
	packet.arp_prolen=4;
	packet.arp_op=htons(1);
	memcpy(packet.arp_src_mac,local_mac,6);
	memcpy(packet.arp_src_ip,local_ip,4);
	memcpy(packet.arp_dst_mac,arp_mac,6);
	memcpy(packet.arp_dst_ip,dst_ip,4);

	struct sockaddr_ll packet_in;
	//bzero(&packet_in,sizeof(packet_in));
	memset(&packet_in,0,sizeof(struct sockaddr_ll));
	packet_in.sll_family=PF_PACKET;
	packet_in.sll_ifindex=if_nametoindex("wlan0");
	packet_in.sll_protocol=htons(ETH_P_ARP);

	packet_in.sll_addr[0]=1;
	packet_in.sll_addr[1]=1;
	packet_in.sll_addr[2]=1;
	packet_in.sll_addr[3]=1;
	packet_in.sll_addr[4]=1;
	packet_in.sll_addr[5]=1;


	printf("%d\n",packet_in.sll_ifindex);

	int fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP));

	int i=0;
	while(1)
	{
		i++;
		int r=sendto(fd,&packet,sizeof(PACKET),0,(struct sockaddr *)&packet_in,sizeof(struct sockaddr));
		if(r<0)
		{
			perror("sendto");
			printf("error=%d\n",errno);
			sleep(1);
		}
		else
		{
			sleep(1);
			printf("%d:%d\n",i,r);
		}
	}
}
