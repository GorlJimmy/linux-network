#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thrd_func1(void *arg);
void *thrd_func2(void *arg);

pthread_t tid1,tid2;

int main(){
    // ....tid1.....thrd_func1
    if (pthread_create(&tid1,NULL,thrd_func1,NULL)!=0) {
        printf("Create thread 1 error!\n");
        exit(1);
    }
    // ....tid2.....thrd_func2
    if (pthread_create(&tid2,NULL,thrd_func2,NULL)!=0) {
        printf("Create thread 2 error!\n");
        exit(1);
    }
    // ....tid1..
    if (pthread_join(tid1,NULL)!=0){
        printf("Join thread 1 error!\n");
        exit(1);
    }else
        printf("Thread 1 Joined!\n");
    // ....tid2..
    if (pthread_join(tid2,NULL)!=0){
        printf("Join thread 2 error!\n");
        exit(1);
    }else
        printf("Thread 2 Joined!\n");

    return 0;
}

void *thrd_func1(void *arg){
//    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL); // ........cancel....

    while(1) {
        printf("Thread 1 is running!\n");
        sleep(1);
    }
    pthread_exit((void *)0);
}

void *thrd_func2(void *arg){
    printf("Thread 2 is running!\n");
    sleep(5);
    if (pthread_cancel(tid1)==0)  // ..tid2...tid1..cancel
        printf("Send Cancel cmd to Thread 1.\n");
        
    pthread_exit((void *)0);
}
