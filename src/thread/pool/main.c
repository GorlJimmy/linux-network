#include "pool.h"

void* work(void* arg)
{
    char *p = (char*) arg;
    printf("thread_pool callback fuction : %s.\n", p);
    sleep(1);
}

int main(void)
{
    struct thread_pool *pool = pool_init(10, 20);
   
  	pool_add_job(pool, work, "1");
	pool_add_job(pool, work, "2");
	pool_add_job(pool, work, "3");
	pool_add_job(pool, work, "4");
	pool_add_job(pool, work, "5");
	pool_add_job(pool, work, "6");
	pool_add_job(pool, work, "7");
	pool_add_job(pool, work, "8");
	pool_add_job(pool, work, "9");
	pool_add_job(pool, work, "10");
	pool_add_job(pool, work, "11");
	pool_add_job(pool, work, "12");
	pool_add_job(pool, work, "13");
	pool_add_job(pool, work, "14");
	pool_add_job(pool, work, "15");
	pool_add_job(pool, work, "16");
	pool_add_job(pool, work, "17");
	pool_add_job(pool, work, "18");
	pool_add_job(pool, work, "19");
	pool_add_job(pool, work, "20");
	pool_add_job(pool, work, "21");
	pool_add_job(pool, work, "22");
	pool_add_job(pool, work, "23");
	pool_add_job(pool, work, "24");
	pool_add_job(pool, work, "25");
	pool_add_job(pool, work, "26");
	pool_add_job(pool, work, "27");
	pool_add_job(pool, work, "28");
	pool_add_job(pool, work, "29");
	pool_add_job(pool, work, "30");
	pool_add_job(pool, work, "31");
	pool_add_job(pool, work, "32");
	pool_add_job(pool, work, "33");
	pool_add_job(pool, work, "34");
	pool_add_job(pool, work, "35");
	pool_add_job(pool, work, "36");
	pool_add_job(pool, work, "37");
	pool_add_job(pool, work, "38");
	pool_add_job(pool, work, "39");
	pool_add_job(pool, work, "40");

    sleep(5);
    pool_destroy(pool);
    return 0;
}
