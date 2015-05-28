#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

int main(){

	pthread_t thread_id;
	thread_id=pthread_self();
	printf("Thread ID: %lu.\n",thread_id);
	if(pthread_equal(thread_id,pthread_self())){
		printf("equal!\n");
	}else{
		printf("not equal\n");
	}
	return 0;
}
