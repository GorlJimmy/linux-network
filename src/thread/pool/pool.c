#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<assert.h>

#include "pool.h"

struct thread_pool* pool_init(int thread_num,int queue_max_num ){
	struct thread_pool *pool = NULL;
	do{
		pool=malloc(sizeof(struct thread_pool));
		if(NULL==pool){
			printf("failed to malloc thread_pool!\n");
			break;
		}
		pool->thread_num=thread_num;
		pool->queue_max_num=queue_max_num;
		pool->queue_cur_num=0;
		pool->head=NULL;
		pool->tail=NULL;
		if(pthread_mutex_init(&(pool->mutex),NULL)){
			printf("failed to init mutex!\n");
			break;
		}
		if(pthread_cond_init(&(pool->queue_empty),NULL)){
			printf("failed to init queue_empty!\n");
			break;
		}
		if (pthread_cond_init(&(pool->queue_not_full), NULL)){
           	printf("failed to init queue_not_full!\n");
            break;
       	}
        pool->pthreads = malloc(sizeof(pthread_t) * thread_num);
        
        if (NULL == pool->pthreads) {
            printf("failed to malloc pthreads!\n");
       		break;
        }
        pool->queue_close = 0;
        pool->pool_close = 0;
        int i=0;
       	for (;i < pool->thread_num; ++i) {
            pthread_create(&(pool->pthreads[i]), NULL,thread_func, (void *)pool);
       	 }
        
        return pool;    
   	 } while (0);
    
   	 return NULL;
	}


int pool_add_job(struct thread_pool* pool, void* (*callback)(void *arg), void *arg)
{
        assert(pool != NULL);
        assert(callback != NULL);
        assert(arg != NULL);

        pthread_mutex_lock(&(pool->mutex));
        while ((pool->queue_cur_num == pool->queue_max_num) && !(pool->queue_close || pool->pool_close))
        {
            pthread_cond_wait(&(pool->queue_not_full), &(pool->mutex));   
        }
        if (pool->queue_close || pool->pool_close)    
        {
            pthread_mutex_unlock(&(pool->mutex));
            return -1;
        }
        struct job *pjob =(struct job*) malloc(sizeof(struct job));
        if (NULL == pjob)
        {
            pthread_mutex_unlock(&(pool->mutex));
            return -1;
        } 
        pjob->callback= callback;    
        pjob->arg = arg;
        pjob->next = NULL;
        if (pool->head == NULL)   
        {
            pool->head = pool->tail = pjob;
            pthread_cond_broadcast(&(pool->queue_not_empty)); 
        }
        else
        {
            pool->tail->next = pjob;
            pool->tail = pjob;    
        }
        pool->queue_cur_num++;
        pthread_mutex_unlock(&(pool->mutex));
        return 0;
}

void* thread_func(void* arg)
{
    struct thread_pool *pool = (struct thread_pool*)arg;
    struct job *pjob = NULL;
    while (1)  //...
    {
        pthread_mutex_lock(&(pool->mutex));
        while ((pool->queue_cur_num == 0) && !pool->pool_close)   //.............
        {
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->mutex));
        }
        if (pool->pool_close)   //...........
        {
            pthread_mutex_unlock(&(pool->mutex));
            pthread_exit(NULL);
        }
        pool->queue_cur_num--;
        pjob = pool->head;
        if (pool->queue_cur_num == 0)
        {
            pool->head = pool->tail = NULL;
        }
        else 
        {
            pool->head = pjob->next;
        }
        if (pool->queue_cur_num == 0)
        {
            pthread_cond_signal(&(pool->queue_empty));        
        }
        if (pool->queue_cur_num == pool->queue_max_num - 1)
        {
            pthread_cond_broadcast(&(pool->queue_not_full));  
        }
        pthread_mutex_unlock(&(pool->mutex));
        
        (*(pjob->callback))(pjob->arg);   
        free(pjob);
        pjob = NULL;    
    }
}
int pool_destroy(struct thread_pool *pool)
{
    assert(pool != NULL);
    pthread_mutex_lock(&(pool->mutex));
    if (pool->queue_close || pool->pool_close){   
        pthread_mutex_unlock(&(pool->mutex));
        return -1;
    }
    
    pool->queue_close = 1;       
    while (pool->queue_cur_num != 0)
    {
        pthread_cond_wait(&(pool->queue_empty), &(pool->mutex));  
    }    
    
    pool->pool_close = 1;      
    pthread_mutex_unlock(&(pool->mutex));
    pthread_cond_broadcast(&(pool->queue_not_empty));  
    pthread_cond_broadcast(&(pool->queue_not_full));   
    int i;
    for (i = 0; i < pool->thread_num; ++i)
    {
        pthread_join(pool->pthreads[i], NULL);   
    }
    
    pthread_mutex_destroy(&(pool->mutex));          
    pthread_cond_destroy(&(pool->queue_empty));
    pthread_cond_destroy(&(pool->queue_not_empty));   
    pthread_cond_destroy(&(pool->queue_not_full));    
    free(pool->pthreads);
    struct job *pjob;
    while (pool->head != NULL)
    {
        pjob = pool->head;
        pool->head = pjob->next;
        free(pjob);
    }
    free(pool);
    return 0;
}
