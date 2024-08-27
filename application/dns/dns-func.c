#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/socket.h>


int main()
{
	char hostname[100];
	puts("input hostname:");
	scanf("%s",hostname);
	struct hostent *ht=NULL;
	char str[30];

	ht=gethostbyname(hostname);

	if(ht)
	{
		int i=0;
		printf("hostname:%s\n",hostname);
		printf("name:%s\n",ht->h_name);
		printf("type:%s\n",ht->h_addrtype==AF_INET?"AF_INET":"AF_INET6");
		printf("length:%d\n",ht->h_length);

		for(i=0;;i++)
		{
			if(ht->h_addr_list[i]!=NULL)
			{
				printf("IP:%s\n",inet_ntop(ht->h_addrtype,ht->h_addr_list[i],str,30));
			}
			else
			{
				break;
			}
		}

		for(i=0;;i++)
		{
			if(ht->h_aliases[i]!=NULL)
			{
				printf("alias %d:%s\n",i,ht->h_aliases[i]);
			}
			else
			{
				break;
			}
		}
	}
}
