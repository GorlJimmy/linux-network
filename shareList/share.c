#include<linux/init.h>
#include<linux/sched.h>
#include<linux/list.h>
#include<linux/semaphore.h>
#include<linux/gfp.h>
#include<linux/slab.h>
#include<linux/module.h>
#include<linux/kthread.h>
#define NTHREADS 100 //thread size
struct my_struct {  
struct list_head list;  
int id;  
int pid;  
};  
static struct work_struct queue; /* 定义工作队列 */  
static struct timer_list mytimer; /* 定时器队列 */  
static LIST_HEAD(mine); /* sharelist头 */  
static unsigned int list_len = 0;  
static DEFINE_SEMAPHORE(sem); /* 内核线程进行同步的信号量 */  
static DEFINE_SPINLOCK(my_lock); /* 保护对链表的操作 */  
static atomic_t my_count = ATOMIC_INIT(0); /* 以原子方式进行追加 */  
static long count = 0; /* 行计数器，每行打印4个信息 */  
static int timer_over = 0; /* 定时器结束标志 */  
static int sharelist(void *data); /*从共享链表增删节点的线程*/  
static void kthread_launcher(struct work_struct *q); /*创建内核线程*/  
static void start_kthread(void); /*调度内核线程 */  
static int sharelist(void *data)  
{  
struct my_struct *p;  
if (count++ % 4 == 0)  
printk("/n");  
spin_lock(&my_lock); /* 添加锁，保护共享资源 */  
if (list_len < 100) {  
if ((p = kmalloc(sizeof(struct my_struct), GFP_KERNEL)) == NULL)  
return -ENOMEM;  
p->id = atomic_read(&my_count); /* 原子变量操作 */  
atomic_inc(&my_count);  
p->pid = current->pid;  
list_add(&p->list, &mine); /* 向队列中添加新节点 */  
list_len++;  
printk("THREAD ADD:%-5d\n", p->id);  
} else { /* 队列超过定长则删除节点 */  
struct my_struct *my = NULL;  
my = list_entry(mine.prev, struct my_struct, list);  
list_del(mine.prev); /* 从队列尾部删除节点 */  
list_len--;  
printk("THREAD DEL:%-5d\n", my->id);  
kfree(my);  
}  
spin_unlock(&my_lock);  
return 0;  
}  
void kthread_launcher(struct work_struct *q)  
{  
kthread_run(sharelist, NULL, "RUNNING..."); /*创建内核线程*/  
up(&sem);  
}  
static void start_kthread(void)  
{  
down(&sem);  
schedule_work(&queue);/*调度工作队列*/  
}  
void qt_task(unsigned long data)  
{  
if (!list_empty(&mine)) {  
struct my_struct *i;  
if (count++ % 4 == 0)  
printk("\n");  
i = list_entry(mine.next, struct my_struct, list); /* 取下一个节点 */  
list_del(mine.next); /* 删除节点 */  
list_len--;  
printk("TIMER DEL:%-5d\n", i->id);  
kfree(i);  
}  
mod_timer(&mytimer, jiffies + 1); /*修改定时器时间*/  
}  
static int __init share_init(void)  
{  
int i;  
printk(KERN_INFO"share list enter\n");  
INIT_WORK(&queue, kthread_launcher);//初始化工作队列  
setup_timer(&mytimer, qt_task, 0); //设置定时器  
add_timer(&mytimer); //添加定时器  
for (i=0;i<NTHREADS;i++) //再启动200个内核线程来添加节点  
start_kthread();  
return 0;  
}  
static void __exit share_exit(void)  
{  
struct list_head *n, *p = NULL;  
struct my_struct *my = NULL;  
printk("/nshare list exit\n");  
del_timer(&mytimer);  
spin_lock(&my_lock); /* 上锁，以保护临界区 */  
list_for_each_safe(p, n, &mine) { /* 删除所有节点，销毁链表 */  
if (count++ % 4 == 0)  
printk("\n");  
my = list_entry(p, struct my_struct, list); /* 取下一个节点 */  
list_del(p);  
printk("SYSCALL DEL: %d\n", my->id);  
kfree(my);  
}  
spin_unlock(&my_lock); /* 开锁 */  
printk(KERN_INFO"Over \n");  
}  
module_init(share_init);  
module_exit(share_exit);  
MODULE_LICENSE("GPL");   

