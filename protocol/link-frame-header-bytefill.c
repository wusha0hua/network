#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 102400

int MTU;
char SON,EOT,escape;

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

char *SendFrame(char *data)
{
	int l,frame_len,i,j,offset;

	l=strlen(data);

	char *tmp=(char*)malloc(LEN);

	for(i=0,j=-1,offset=0;i<l;)
	{
		if(offset%MTU==0)
		{
			offset=0;
			j++;
			tmp[j*MTU+offset]=SON;
			offset++;
		}
		else if(offset%(MTU-1)==0)
		{
			tmp[j*MTU+offset]=EOT;
			offset++;
		}
		else 
		{
			if(data[i]==SON || data[i]==EOT || data[i]==escape)
			{
				if(offset!=MTU-2)
				{
					tmp[j*MTU+offset]=escape;
					offset++;
					tmp[j*MTU+offset]=data[i];
					i++;
				}
				else
				{
					tmp[j*MTU+offset]=EOT;
					j++;
					tmp[j*MTU]=SON;
					offset=1;
					tmp[j*MTU+offset]=data[i];
					i++;
				}
			}
			else
			{
				tmp[j*MTU+offset]=data[i];
				i++;
				offset++;
			}
		}
	}

	frame_len=j*MTU+offset;

	char *frame=(char *)malloc(frame_len);

	memcpy(frame,tmp,frame_len);

	free(tmp);

	return frame;
}


int main()
{
	printf("input SON : ");
	scanf("%x",&SON);
	printf("input EOT : ");
	scanf("%x",&EOT);
	printf("input escape : ");
	scanf("%x",&escape);
	printf("input MTU : ");
	scanf("%d",&MTU);
	

	char data[LEN];
	memset(data,0,LEN);

	printf("input data : ");
	scanf("%s",data);	

	char *sendframe=SendFrame(data);

	PrintHex(sendframe);
}
