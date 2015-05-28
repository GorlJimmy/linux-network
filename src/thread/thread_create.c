#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

void *thread_func(void *arg);
pthread_t tid;

int main(){

	if(pthread_create(&tid,NULL,thread_func,NULL)!=0){
		printf("error!\n");
		exit(1);
	}else{
		printf("TID in pthread_create function: %u.\n",tid);
		printf("Main process: PID: %d,thread ID:%u.\n",getpid(),pthread_self());
		sleep(1);
	}
	return 0;
}

void *thread_func(void *arg){
	printf("New Process thread: Process ID:%d,Thread ID:%u.\n",getpid(),pthread_self());
	printf("New Process thread: Process ID:%d,Thread ID:%u.\n",getpid(),tid);
	
	pthread_exit(NULL);
}


