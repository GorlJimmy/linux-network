#include<unistd.h>
#include<signal.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>

#include "daemon.h"

void setDaemon(void){
	int pid;
	int i=0;	

	if(pid=fork())
		exit(0);
	else if(pid<0)
		exit(1);
	setsid();

	if(pid=fork())
		exit(0);
	else if(pid<0)
		exit(1);

	for(;i<NOFILE;i++){
		close(i);
	}
	
	chdir("/tmp");
	umask(0);
	return ;
}
