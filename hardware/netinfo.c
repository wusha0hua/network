#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<linux/if.h>
#include<errno.h>
#include<sys/ioctl.h>

typedef struct 
{
	char name[10];
	int index;
	unsigned char mac[6];
	unsigned char ip[4];
	int mtu;
}LNDI;

void GetLocalNetInfo(LNDI *lndi)
{
	struct ifreq ifr;
	memset(&ifr,0,sizeof(struct ifreq));

	int fd=socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0)
	{
		printf("socket");
		perror("socket");
	}

	memcpy(ifr.ifr_ifrn.ifrn_name,lndi->name,sizeof(ifr.ifr_ifrn));

	int ioctl_ip=ioctl(fd,SIOCGIFADDR,&ifr);
	if(ioctl_ip<0)
	{
		puts("ioctl ip error");
		exit(0);
	}
	memcpy(lndi->ip,ifr.ifr_ifru.ifru_addr.sa_data+2,4);

	int ioctl_mac=ioctl(fd,SIOCGIFHWADDR,&ifr);
	if(ioctl_mac<0)
	{
		puts("ioctl mac error");
		exit(0);
	}
	memcpy(lndi->mac,ifr.ifr_ifru.ifru_hwaddr.sa_data,6);

	ioctl(fd,SIOCGIFMTU,&ifr);
	lndi->mtu=ifr.ifr_mtu;

	if(ioctl(fd,SIOCGIFINDEX,&ifr)==-1)
	{
		puts("index error");
		exit(0);
	}
	lndi->index=ifr.ifr_ifindex;
	close(fd);
}


int main()
{
	LNDI lndi;
	memset(&lndi,0,sizeof(LNDI));
	puts("input device name:");
	scanf("%s",lndi.name);
	
	GetLocalNetInfo(&lndi);

	for(int i=0;i<6;i++)
	{
		printf("%02X ",lndi.mac[i]);
	}
	puts("");

	for(int i=0;i<4;i++)
	{
		printf("%02X ",lndi.ip[i]);
	}
	puts("");

	printf("MTU:%d\n",lndi.mtu);
	/*printf("%d\n",inet_addr("192.168.1.111"));
	printf("%ud\n",(192<<24)|(168<<16)|(1<<8)|(111));
	printf("%ud\n",(111<<24)|(1<<16)|(168<<8)|(192));*/
}
