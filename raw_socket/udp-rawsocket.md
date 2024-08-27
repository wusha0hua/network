
# Header

```c
#include<sys/socket.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<arpa/inet.h>
```

# Pseudo Header

```c
struct UDP_PSD_Header
{
	u_int32_t src_ip;
	u_int32_t dst_ip;
	u_int8_t zero;
	u_int8_t protocol;
	u_int16_t udp_len;
};

struct UDP_PSD_Header *udp_psd_heafer =(struct UDP_PSD_Header *)buf[sizeof(struct UDP_PSD_Header)+sizeof(struct udphdr)+sizeof(data)];
udp_psd_heafer->src_ip=src_ip;
udp_psd_heafer->dst_ip=dst_ip;
udp_psd_heafer->zero=0;
udp_psd_heafer->protocol=IPPROTO_UDP;
udp_psd_heafer->udp_len=htons(sizeof(struct udphdr)+sizeof(data));
```

# CheckSum

```c
unsigned short CheckSum(char *buf,int len)
{
	unsigned long sum;
	for(sum=0;len>0;len--)
	{
		sum+=*buf++;
	}
	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	return (unsigned short)(~sum);
}
```
# packet

```c
char *packet[LEN];
memset(packet,0,LEN);
```

# ip packet

```c
struct iphdr *ip;

ip->ihl=5;
ip->version=4;
ip->tos=0;
ip->tot_len=sizeof(struct iphdr)+sizeof(struct udphdr)+sizeof(data);
ip->ttl=255;
ip->protocol=IPPROTO_UDP;
ip->check=0;
ip->saddr=;
ip->daddr=;


```

# udp packet

```c
struct udphdr *udp=(struct udphdr*)(packet+sizeof(struct iphdr));

udp->source=src_port;
udp->dest=dst_port;
udp->len=htons(sizeof(struct udphdr)+sizeof(data));
udp->check=CheckSum((char*)buf,sizeof(struct udphdr)+sizeof(struct UDP_PSD_Header)+sizeof(data));
```

# socket

```c
int sock=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
const int val=1;
setsockopt(sock,IPPROTO_IP,IP_HDRINCL,&val,sizeof(val));
```
