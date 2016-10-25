

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

struct task_struct *ts;
int id = 101;
bool threadStop = false;
static int count;

int function(void *data){
   int n = *(int*)data;
   while(count < 10){
    printk(KERN_EMERG "DING\n");    
    set_current_state(TASK_INTERRUPTIBLE);
    schedule_timeout(HZ);
    threadStop = kthread_should_stop();
    if(threadStop){
	return 0; 
    }
    count++;
   }     
   return 0;
}

int init_module( void ) {
   count = 1;
   printk("Kthread timer module installing\n");
   ts = kthread_run(function, (void*)&id, "spawn"); 
   return 0;
}


void cleanup_module( void ) {
    if(!threadStop) kthread_stop(ts);
    printk("Kthread module uninstalling\n");
    return;
}
