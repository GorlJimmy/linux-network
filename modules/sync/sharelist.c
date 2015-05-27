#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/semaphore.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kthread.h>

#define NTHREADS 100 //thread size
struct share_struct {  
	struct list_head list;  
	int id;  
	int pid;  
};  
static struct work_struct queue; /* work queue */ 

static struct timer_list qt_timer; /* timer queue */  

static LIST_HEAD(l_head); /* share list head */  

static unsigned int list_len = 0;  

static DEFINE_SEMAPHORE(sem); /* Kernel threads semaphore */  

static DEFINE_SPINLOCK(pro_lock); /* lock: protect the list */  

static atomic_t ato_count = ATOMIC_INIT(0);   

static long count = 0;   

static int timer_over = 0;  

static int sharelist(void *data); /* delete node from the shared list */  

static void kthread_launcher(struct work_struct *q); /* create kernel thread */  

static void start_kthread(void); /* Scheduling kernel thread */  

static int sharelist(void *data)  
{  
	struct share_struct *p;  
	if (count++ % 4 == 0)  
		printk("\n");  
	spin_lock(&pro_lock); /* add lock to protect the shared resource */  
	if (list_len < 100) {  
		if ((p = kmalloc(sizeof(struct share_struct), GFP_KERNEL)) == NULL)  
			return -ENOMEM;  
		p->id = atomic_read(&ato_count); 
		atomic_inc(&ato_count);  
		p->pid = current->pid;  
		list_add(&p->list, &l_head); /* add new node */  
		list_len++;  
		printk("THREAD ADD:%-5d\n", p->id);  
} else {  
		struct share_struct *s_struct = NULL;  
		s_struct = list_entry(l_head.prev, struct share_struct, list);  
		list_del(l_head.prev); /* delete node from shared list */  
		list_len--;  
		printk("THREAD DEL:%-5d\n", s_struct->id);  
		kfree(s_struct);  
	}  
	spin_unlock(&pro_lock);  
	return 0;  
}  
void kthread_launcher(struct work_struct *q)  
{  
	kthread_run(sharelist, NULL,"RUNNING...");  /* create new kernel thread */  
	//kernel_thread(sharelist, NULL, CLONE_KERNEL | SIGCHLD);
	up(&sem);  
}  
static void start_kthread(void)  
{  
	down(&sem);  
	schedule_work(&queue); /* Scheduling work queue*/  
}  
void qt_task(unsigned long data)  
{  
	if(timer_over==0){
		if (!list_empty(&l_head)) {  
			struct share_struct *i;  
			if (count++ % 4 == 0)  
				printk("\n");  
			i = list_entry(l_head.next, struct share_struct, list); /* get next node */  
			list_del(l_head.next); /* delete node */  
			list_len--;  
			printk("TIMER DEL:%-5d\n", i->id);  
			kfree(i);  
		}
	}
	mod_timer(&qt_timer, jiffies + 1);   
}  
static int __init share_init(void)  
{  
	int i;  
	printk(KERN_INFO"share list enter\n");  
	INIT_WORK(&queue, kthread_launcher);//init work queue
	setup_timer(&qt_timer, qt_task, 0); //setting timer
	add_timer(&qt_timer); //add timer
	for (i=0;i<NTHREADS;i++) //start 200 thread for adding nodes to list 
		start_kthread();  
	return 0;  
}  
static void __exit share_exit(void)  
{  
	struct list_head *n, *p = NULL;  
	struct share_struct *s_struct = NULL;  
	printk("\nshare list exit\n");  
	del_timer(&qt_timer);  
	spin_lock(&pro_lock); 
	list_for_each_safe(p, n, &l_head) { /* delete nodes and destory list */  
	if (count++ % 4 == 0)  
		printk("\n");  
	s_struct = list_entry(p, struct share_struct, list); /* get next node */  
	list_del(p);  
	printk("SYSCALL DEL: %d\n", s_struct->id);  
	kfree(s_struct);  
}  
	spin_unlock(&pro_lock); /* unlock */  
	printk(KERN_INFO"Over \n");  
}  
module_init(share_init);  
module_exit(share_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gao Xiaoning");  

