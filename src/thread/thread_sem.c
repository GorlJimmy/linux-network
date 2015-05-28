#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_NUM 3
#define REPEAT_TIMES 5
#define DELAY 4

sem_t sem[THREAD_NUM];

void *thrd_func(void *arg);

int main(){
    pthread_t thread[THREAD_NUM];
    int no;
    void *tret;
    
    srand((int)time(0)); 

    // ...THREAD_NUM-1..........0
    for(no=0;no<THREAD_NUM-1;no++){
        sem_init(&sem[no],0,0);
    }

    // sem[2].......1..sem..........
    sem_init(&sem[2],0,1);
    
    // ..THREAD_NUM..........thrd_func....(void*)no
    for(no=0;no<THREAD_NUM;no++){
        if (pthread_create(&thread[no],NULL,thrd_func,(void*)no)!=0) {
            printf("Create thread %d error!\n",no);
            exit(1);
        } else
            printf("Create thread %d success!\n",no);
    }
    
    // ..join.THREAD_NUM...
    for(no=0;no<THREAD_NUM;no++){
        if (pthread_join(thread[no],&tret)!=0){
            printf("Join thread %d error!\n",no);
            exit(1);
        }else
            printf("Join thread %d success!\n",no);
    }
    
    // .......
    for(no=0;no<THREAD_NUM;no++){
        sem_destroy(&sem[no]);
    }

    return 0;
}

void *thrd_func(void *arg){
    int thrd_num=(void*)arg; // ..no
    int delay_time,count;

    // .....p..
    sem_wait(&sem[thrd_num]);

    
    printf("Thread %d is starting.\n",thrd_num);
    for(count=0;count<REPEAT_TIMES;count++) {
        delay_time=(int)(DELAY*(rand()/(double)RAND_MAX))+1;
        sleep(delay_time);
        printf("\tThread %d:job %d delay =%d.\n",thrd_num,count,delay_time);
    }

    printf("Thread %d is exiting.\n",thrd_num);
    
    // .........V..
    // ...............1.....0
    // ............
    sem_post(&sem[(thrd_num+THREAD_NUM-1)%THREAD_NUM]);

    pthread_exit(NULL); // ......
}
