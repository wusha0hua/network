/*
 * Raw UDP sockets
 */
#include <stdio.h>       //printf
#include <string.h>      //memset
#include <sys/socket.h>  //socket
#include <stdlib.h>      //exit
#include <errno.h>       //errno
#include <netinet/udp.h> //udp header
#include <netinet/ip.h>  //ip header
#include <arpa/inet.h>   //inet_addr

#define SRCIP "10.107.115.114"
#define DSTIP "123.0.0.3"

/*
 * 96 bit (12 bytes) pseudo header
 * needed for udp header checksum calculation
*/
struct pseudo_header
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t udp_length;
};

/*
 * Generic checksum calculation function
 */
unsigned short csum(unsigned short *ptr, int nbytes)
{
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;

    return(answer);
}

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (s < 0)
        die("socket()");

    char datagram[4096], *pseudogram;
    memset(datagram, 0, 4096);

    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (struct udphdr *)(datagram + sizeof(struct ip));
    char *data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    struct pseudo_header psh;
    char source_ip[32] = SRCIP;

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    sin.sin_addr.s_addr = inet_addr(DSTIP);

    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + 1300;
    iph->id = htonl (54321); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;

    //Ip checksum
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);

    //UDP header
    udph->source = htons (6666);
    udph->dest = htons (8622);
    udph->len = htons(8 + 1300);
    udph->check = 0; //leave checksum 0 now, filled later by pseudo header

    //Now the UDP checksum using the pseudo header
    psh.source_address = inet_addr( source_ip );
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = htons(sizeof(struct udphdr) + 1300);

    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + 1300;
    pseudogram = malloc(psize);

    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + 1300);

    udph->check = csum( (unsigned short*) pseudogram , psize);

    //loop if you want to flood :)
    //while (1)
    {
        //Send the packet
        if (sendto (s, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0) {
            perror("sendto failed");
        }
        //Data send successfully
        else {
            printf ("Packet Send. Length : %d \n" , iph->tot_len);
        }
    }

    return 0;
}
