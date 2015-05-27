#include <linux/init.h>
#include <linux/module.h>
#include <asm/thread_info.h>
#include <linux/sched.h>
  
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BUNFLY");
 
static int __init test_init(void)
 {
       printk("name is %s", current->comm);
struct thread_info *p = NULL;
p = (struct thread_info *)((unsigned long)&p  & ~(8192 - 1));
     
struct task_struct *head = p->task;
struct task_struct *tmp = head;
int i=0;
do{
i++;
    printk("%d name is %s\n",i, tmp->comm);
    tmp = container_of(tmp->tasks.next, struct task_struct, tasks);

 }while(tmp != head);
 
 return 0;
 }
 
static void __exit test_exit(void){
         printk("bye bye");
 }

module_init(test_init);
module_exit(test_exit);
