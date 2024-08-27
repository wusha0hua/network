#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 1024

void PrintHex(char *data)
{
	int i,l;
	l=strlen(data);
	for(i=0;i<l;i++)
	{
		printf("%02X ",data[i]);
	}
	puts("");
}

void PrintBin(char *data)
{
	int i,l,j;
	l=strlen(data);

	for(i=0;i<l;i++)
	{
		for(j=0;j<8;j++)
		{
			if(data[i]&(1<<(7-j)))
			{
				printf("1");
			}
			else
			{
				printf("0");
			}
		}
		puts("");
	}
}

int check(char c)
{
	int i,n=0;
	for(i=0;i<8;i++)
	{
		if(c&1)
		{
			n++;
		}
		c>>=1;
	}
	return n;
	
}

char *EvenParityCode(char *data)
{
	int i,l;
	l=strlen(data);
	
	char *ret=(char*)malloc(l);

	for(i=0;i<l;i++)
	{
		if(check(data[i])%2)
		{
			ret[i]=data[i]|(1<<7);
		}
		else
		{
			ret[i]=data[i];
		}
	}
	return ret;
}

int main()
{
	char data[LEN];
	memset(data,0,LEN);

	printf("input data:");
	scanf("%s",data);
	char *evenparitycode=EvenParityCode(data);
	
	PrintBin(data);
	puts("");
	PrintBin(evenparitycode);
	puts("");
	PrintHex(evenparitycode);
}
