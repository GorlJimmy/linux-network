#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "daemon.h"

void setDaemon(void){

	
	switch(fork()){
		case -1:
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
			exit(1);
			break;
		default:
			exit(0);
			break;
		case 0:
			break;
	}

	int i;	
	for(i=0;i<NOFILE;i++){
		close(i);
	}
	
	chdir("/tmp");
	umask(0);
	return ;
}
