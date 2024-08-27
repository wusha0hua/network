
```c
#include<net/if_arp.h>

struct arphdr
{
	unsigned short int ar_hrd;
	unsigned short int ar_pro;
	unsigned char ar_hln;
	unsigned char ar_pln;
	unsigned short int ar_op;
};
```
```c
struct arp
{
	unsigned short int hwdtype;
	unsigned short int protocol;
	unsigned char hwdlen;
	unsigned prolen;
	unsigned short int op;
	unsigned char src_MAC[6];
	unsigned char src_ip[4];
	unsigned char dst_MAC[6];
	unsigned char dst_ip[4];
}

```
