#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define LEN 102400
#define FRAME_LEN 5

void PrintHex(char *frame)
{
	int i,l;
	l=strlen(frame);
	for(i=0;i<l;i++)
	{
		printf("%02X ",frame[i]);
	}
	puts("");
}

char *SendFrame(char *data)
{
	int i,l,j,k,frame_len;
	
	l=strlen(data);


	if(l%(FRAME_LEN)==0)
	{
		frame_len=l+l/(FRAME_LEN-1);
	}
	else
	{
		frame_len=l+l/(FRAME_LEN-1)+1;
	}

	char *frame=(char*)malloc(frame_len);

	for(i=0,k=0;i<frame_len;i++)
	{
		if(i%FRAME_LEN==0)
		{
			if(i+FRAME_LEN>frame_len)
			{
				frame[i]=frame_len-i;
			}
			else
			{
				frame[i]=FRAME_LEN;
			}
		}
		else
		{
			frame[i]=data[k];
			k++;
		}
	}
	
	return frame;
}

char *RecieveFrame(char *frame)
{
	
	

}

int main()
{
	char data [LEN];
	memset(data,0,LEN);

	printf("input data : ");
	fgets(data,LEN,stdin);

	if(data[strlen(data)-1]=='\n')
	{
		data[strlen(data)-1]='\0';
	}
	
	char *sendframe=SendFrame(data);

	//char *recievedata=RecieveFrame(sendframe);

	PrintHex(sendframe);
	//PrintHex(recievedata);

	puts(sendframe);
	//puts(recievedata)
	return 0;
}
