#ifndef POOL_H
#define POOL_H
#include<pthread.h>
struct job{
	void* (*callback)(void *arg);
	void *arg;
	struct job *next;
};

struct threadPool{
	int thread_num;
	int queue_max_num;
	struct job *head;
	struct job *tail;
	pthread_t *pthreads;
	pthread_mutex_t mutex;
	pthread_cond_t queue_empty;
	pthread_cond_t queue_not_empty;
	pthread_cond_t queue_not_full;
	int queue_cur_num;
	int queue_close;
	int pool_close;
};

struct threadPool* pool_init(int thread_num,int queue_max_num);
int pool_add_job(struct threadPool  *pool,void*(*callback)(void *arg),void *arg);
int pool_destory(struct threadPool *pool);
void* pool_function(void *arg);
#endif
