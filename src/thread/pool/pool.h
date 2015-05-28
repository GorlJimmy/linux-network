#ifndef POOL_H
#define POOL_H
#include<pthread.h>
struct job{
	void* (*callback)(void *arg);
	void *arg;
	struct job *next;
};

struct thread_pool{
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

struct thread_pool* pool_init(int thread_num,int queue_max_num);
int pool_add_job(struct thread_pool  *pool,void*(*callback)(void *arg),void *arg);
int pool_destory(struct thread_pool *pool);
void* thread_func(void *arg);
#endif
