#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<net/if.h>

void GetDeviceInfo()
{
	struct ifconf ifc;
	struct ifreq ifr[10];
	struct ifreq ifrs;


	memset(&ifc,0,sizeof(struct ifconf));

	int fd=socket(AF_INET,SOCK_DGRAM,0);

	ifc.ifc_len=10*sizeof(struct ifreq);
	ifc.ifc_buf=(char*)ifr;

	if(ioctl(fd,SIOCGIFCONF,&ifc)==-1)
	{

	}
	
	int iflen=ifc.ifc_len/(sizeof(struct ifreq));
	char **device=(char **)malloc(sizeof(char*)*iflen);


	for(int i=0;i<iflen;i++)
	{
		device[i]=(char*)malloc(20);
		memcpy(device[i],ifr[i].ifr_ifrn.ifrn_name,strlen(ifr[i].ifr_ifrn.ifrn_name));
	}

	//char **ip=(char **)malloc(iflen*sizeof(char *));
	unsigned char ip[6];
	for(int i=0;i<iflen;i++)
	{
		memset(&ifrs,0,sizeof(struct ifreq));
		//ip[i]=(char *)malloc(INET_ADDRSTRLEN);
		memcpy(&ifrs,device[i],strlen(device[i]));
		ioctl(fd,SIOCGIFADDR,&ifrs);

		memcpy(ip,ifrs.ifr_ifru.ifru_addr.sa_data,6);
		for(int j=0;j<6;j++)
		{
			printf("%02X ",ip[i]);
		}
	
	}

	for(int i=0;i<iflen;i++)
	{
		printf("name:%s\n",device[i]);
		printf("\tip:%s\n",ip[i]);
	}


	close(fd);
}

int main()
{

	GetDeviceInfo();
	/*struct ifconf ifc;
	struct ifreq ifr[10];

	char ip[INET_ADDRSTRLEN]={'\0'};

	memset(&ifc,0,sizeof(struct ifconf));

	int fd=socket(AF_INET,SOCK_DGRAM,0);

	ifc.ifc_len=10*sizeof(struct ifreq);
	ifc.ifc_buf=(char *)ifr;

	if(ioctl(fd,SIOCGIFCONF,&ifc)==-1)
	{
		puts("error");
		exit(-1);
	}
	
	int iflen=ifc.ifc_len/(sizeof(struct ifreq));

	for(int i=0;i<iflen;i++)
	{

		printf("%s:\n",ifr[i].ifr_ifrn.ifrn_name);
		
		ioctl(fd,SIOCGIFADDR,&ifr[i]);
		memcpy(ip,ifr[i].ifr_ifru.ifru_addr.sa_data+2,4);
		for(int j=0;j<4;j++)
		{
			printf("%02X ",(unsigned char)ip[i]);
		}


		puts("");

		printf("%s:",ifr[i].ifr_name);
		inet_ntop(AF_INET,&(((struct sockaddr_in *)&(ifr[i].ifr_addr))->sin_addr),ip,INET_ADDRSTRLEN);
		printf("\tip:%s\n",ip);
		
		printf("\tmac:%s\n",ifr[i].ifr_hwaddr.sa_data);

		puts("");
	}

	close(fd);*/
}
