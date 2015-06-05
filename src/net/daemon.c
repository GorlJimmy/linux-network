#include<unistd.h>
#include<signal.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>

#include "daemon.h"

void set_daemon(void){
	int pid;
	int i=0;	
	switch(fork()){
		case -1:
			perror("fork error");
			exit(1);
			break;
		default:
			exit(0);
			break;
		case 0:
			break;
	}
	setsid();
	switch(fork()){
		case -1:
			perror("fork error");
			exit(1);
			break;
		default:
			exit(0);
			break;
		case 0:
			break;
	}

	for(;i<NOFILE;i++){
		close(i);
	}
	
	chdir("/tmp");
	umask(0);
	return ;
}
