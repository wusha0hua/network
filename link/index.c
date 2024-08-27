#include<stdio.h>
#include<sys/socket.h>
#include<linux//if_ether.h>
#include<arpa//inet.h>
#include<unistd.h>
#include<error.h>
#include<linux/if_ether.h>
#include<string.h>
#include <sys/ioctl.h>  
#include <fcntl.h>  
#include <net/if.h> 
#include<stdlib.h>
int main()
{
	int fd;
	fd = socket(AF_PACKET,SOCK_DGRAM,htons(ETH_P_ARP));

	char *if_name="wlan0";

	struct ifreq ifr;
	size_t if_name_len = strlen(if_name);
	if(if_name_len < sizeof(ifr.ifr_name)) 
	{
		memcpy(ifr.ifr_name,if_name,if_name_len);
							     
	}
	else 
	{
		perror("interface name is too long");
	
	}
	if(-1 == ioctl(fd,SIOCGIFINDEX,&ifr))
	{
		exit(0);
	}
	int ifindx=1000;
	ifindx = ifr.ifr_ifindex;
	printf("%d\n",ifindx);

}

