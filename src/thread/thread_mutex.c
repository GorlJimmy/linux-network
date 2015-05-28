#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 3
#define REPEAT_TIMES 5
#define DELAY 4

pthread_mutex_t mutex;

void *thrd_func(void *arg);

int main(){
    pthread_t thread[THREAD_NUM];
    int no;
    void *tret;
    
    srand((int)time(0));
       // ...................mutex    
    pthread_mutex_init(&mutex,NULL);

    // ..THREAD_NUM............&thread[no]............thrd_func....
    for(no=0;no<THREAD_NUM;no++){
        if (pthread_create(&thread[no],NULL,thrd_func,(void*)no)!=0) {
            printf("Create thread %d error!\n",no);
            exit(1);
        } else
            printf("Create thread %d success!\n",no);
    }
    
    // .......join.....tret
    for(no=0;no<THREAD_NUM;no++){
        if (pthread_join(thread[no],&tret)!=0){
            printf("Join thread %d error!\n",no);
            exit(1);
        }else
            printf("Join thread %d success!\n",no);
    }
    // .....
    pthread_mutex_destroy(&mutex);
    return 0;
}

void *thrd_func(void *arg){
    int thrd_num=(void*)arg; // ............
    int delay_time,count; 
    
    // ......
    if(pthread_mutex_lock(&mutex)!=0) {
        printf("Thread %d lock failed!\n",thrd_num);
        pthread_exit(NULL);
    }

    printf("Thread %d is starting.\n",thrd_num);
    for(count=0;count<REPEAT_TIMES;count++) {
        delay_time=(int)(DELAY*(rand()/(double)RAND_MAX))+1;
        sleep(delay_time);
        printf("\tThread %d:job %d delay =%d.\n",thrd_num,count,delay_time);
    }

    printf("Thread %d is exiting.\n",thrd_num);
    // ..
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}
