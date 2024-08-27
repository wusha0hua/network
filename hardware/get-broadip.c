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

int get_eth_ip(char *eth_name,unsigned char *ip)
{
	struct ifreq ifr;
	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		puts("socket error");
		exit(0);
	}

	strncpy(ifr.ifr_ifrn.ifrn_name,(char*)eth_name,sizeof(ifr.ifr_ifrn));
	
	int ret_ioctl=ioctl(fd,SIOCGIFBRDADDR,&ifr);

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
	memcpy(ip,ifr.ifr_ifru.ifru_addr.sa_data+2,4);
	close(fd);
	return 0;
}

int main()
{
	char device[20];
	unsigned char local_ip[4];
	puts("input the device name:");
	scanf("%s",device);
	get_eth_ip(device,local_ip);

	int i;
	
	for(i=0;i<3;i++)
	{
		printf("%d.",local_ip[i]);
	}
	printf("%d\n",local_ip[i]);
}
