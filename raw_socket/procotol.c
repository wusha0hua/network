#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet//ether.h>
#include<netinet/icmp6.h>
#include<netinet/igmp.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip6.h>
#include<netinet/ip_icmp.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>

int main()
{
	printf("\nip:%d",IPPROTO_IP);
	printf("\nicmp:%d",IPPROTO_ICMP);
	printf("\nipv6:%d",IPPROTO_IPV6);
	printf("\nethernet:%d",IPPROTO_ETHERNET);
	printf("\ntcp:%d",IPPROTO_TCP);
	printf("\nudp:%d",IPPROTO_UDP);
	printf("\nigmp:%d",IPPROTO_IGMP);

	puts("");
}
